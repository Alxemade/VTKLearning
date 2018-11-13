#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageCast.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkGeometryFilter.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkStripper.h>
#include <vtkThreshold.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

/* canny 检测
1. 使用gauss滤波器进行平滑
2. 计算图像的梯度大小和方向
3. 进行极大值抑制，细化边缘
4. 利用双阈值进行边缘检测和边缘连接
5. over
*/

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName( "hua2.jpg" );
	reader->Update();  // 首先读取一幅图像数据

	vtkSmartPointer<vtkImageCast> ic =
		vtkSmartPointer<vtkImageCast>::New();  // Image Data type Casting Filter
	ic->SetOutputScalarTypeToDouble();  // 将输出图像的数据变成float类型，为什么？
	// 这里猜想的是因为进行高斯滤波的时候需要和的gauss函数卷积，而这个是浮点型数据，所以我们需要将原图像类型也转换为浮点型数据
	ic->SetInputConnection(reader->GetOutputPort());
	// step.1
	vtkSmartPointer<vtkImageGaussianSmooth> gs =
		vtkSmartPointer<vtkImageGaussianSmooth>::New();  //  Performs a gaussian convolution.
	gs->SetInputConnection(ic->GetOutputPort()); // vtk管线连接
	gs->SetDimensionality(2);  // 设置gauss滤波器的维度，这里处理的是二维数据，所以设置维度为2
	gs->SetRadiusFactors(1, 1, 0);  //  Sets/Gets the Radius Factors of the gaussian (no unit).
	
	vtkSmartPointer<vtkImageGradient> imgGradient =
		vtkSmartPointer<vtkImageGradient>::New();  // Computes the gradient vector
	imgGradient->SetInputConnection(gs->GetOutputPort());
	imgGradient->SetDimensionality(2);  // 计算2维的梯度场
	// step.2
	vtkSmartPointer<vtkImageMagnitude> imgMagnitude =
		vtkSmartPointer<vtkImageMagnitude>::New();  // 计算梯度的模值
	imgMagnitude->SetInputConnection(imgGradient->GetOutputPort());
	// step.3
	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMax =
		vtkSmartPointer<vtkImageNonMaximumSuppression>::New(); // Performs non-maximum suppression
	// 一般来说，这个接收二个输入，一个是模值图像，另外一个就是梯度图像
	nonMax->SetMagnitudeInputData(imgMagnitude->GetOutput());  //  Set the magnitude inputs.
	nonMax->SetVectorInputData(imgGradient->GetOutput());  //  Set the vector inputs.
	nonMax->SetDimensionality(2);  // 进行非极大值抑制的输出为2维， Output is magnitude information and is always in doubles.
	// 这里的作用是将梯度图像像素的组分修改为3，方便下面vtkImageToStructuredPoints使用。
	vtkSmartPointer<vtkImageConstantPad> pad =
		vtkSmartPointer<vtkImageConstantPad>::New();  //  Makes image larger by padding with constant
	pad->SetInputConnection(imgGradient->GetOutputPort());
	pad->SetOutputNumberOfScalarComponents(3);  // 设置扩充之后图像的组分为3
	pad->SetConstant(0);  // 设置扩充的值为0
	// 将vtkImageData变成datapoint类型数据
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp1 =
		vtkSmartPointer<vtkImageToStructuredPoints>::New();  // changes an image cache format to a structured points dataset
	i2sp1->SetInputConnection(nonMax->GetOutputPort());  // vtk管线连接
	i2sp1->SetVectorInputData(pad->GetOutput());  
	// step.4
// 类根据点的相邻关系连接成连续的折线Polyline
	vtkSmartPointer<vtkLinkEdgels> imgLink =
		vtkSmartPointer<vtkLinkEdgels>::New();
	imgLink->SetInputData(i2sp1->GetOutput());
	imgLink->SetGradientThreshold(2);  // 设置较小的阈值
	// 获取输入任意类型数据的满足阈值条件的单元数据
	vtkSmartPointer<vtkThreshold> thresholdEdgels =
		vtkSmartPointer<vtkThreshold>::New();  // extracts cells where scalar value in cell satisfies threshold criterion
	thresholdEdgels->SetInputConnection(imgLink->GetOutputPort());
	thresholdEdgels->ThresholdByUpper(10); // 设置较大的阈值
	thresholdEdgels->AllScalarsOff();
	// 将数据转换为几何数据
	vtkSmartPointer<vtkGeometryFilter> gf =
		vtkSmartPointer<vtkGeometryFilter>::New(); // extract geometry from data (or convert data to polygonal type)
	gf->SetInputConnection(thresholdEdgels->GetOutputPort());
	// 将vtkImageData变成datapoint类型数据
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp =
		vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp->SetInputConnection(imgMagnitude->GetOutputPort());
	i2sp->SetVectorInputData(pad->GetOutput());
	// 接收一系列连续曲线及其对应的梯度系信息作为输入，利用梯度信息来调整曲线位置
	vtkSmartPointer<vtkSubPixelPositionEdgels> spe =
		vtkSmartPointer<vtkSubPixelPositionEdgels>::New();  //  adjust edgel locations based on gradients.
	spe->SetInputConnection(gf->GetOutputPort());
	spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());
	// vtkStripper用来将输入的多边形、三角形或者线段生成三角形带或者折线段
	vtkSmartPointer<vtkStripper> strip =
		vtkSmartPointer<vtkStripper>::New();  //  create triangle strips and/or poly-lines
	strip->SetInputConnection(spe->GetOutputPort());
	strip->Update();
	// 这里vtkPolyDataMapper作用就是显示vktPolyData数据类型
	vtkSmartPointer<vtkPolyDataMapper> dsm =
		vtkSmartPointer<vtkPolyDataMapper>::New();  // map vtkPolyData to graphics primitives
	dsm->SetInputConnection(strip->GetOutputPort());
	dsm->ScalarVisibilityOff();

	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();   
	inputActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkActor> extractedActor =
		vtkSmartPointer<vtkActor>::New();
	extractedActor->SetMapper(dsm);
	extractedActor->GetProperty()->SetAmbient(1.0);  // 设置光照
	extractedActor->GetProperty()->SetDiffuse(0.0);  // 设置diffuse
	extractedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);  // 设置颜色

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 300);  // 设置渲染窗口的大小，600*300

	// And one interactor， 仅仅使用一个渲染窗口交互器
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

	// Setup both renderers，虽然使用了一个渲染窗口，但是我们需要设置二个渲染器
	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建左边的渲染器
	renderWindow->AddRenderer(leftRenderer);  // 首先我们将我们左边的渲染器添加到渲染窗口里面去
	leftRenderer->SetViewport(leftViewport);  // 设置我们的左边渲染器的视窗
	leftRenderer->SetBackground(1.0, 1.0, 1.0);  // 设置左边的渲染器的背景颜色

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建右边的渲染器
	renderWindow->AddRenderer(rightRenderer); // 将右边渲染器添加到渲染窗口里面
	rightRenderer->SetViewport(rightViewport); // 设置渲染器的视窗
	rightRenderer->SetBackground(1.0, 1.0, 1.0);  // 设置渲染器的背景

	leftRenderer->AddActor(inputActor);  // 给左侧的渲染器添加Actor
	rightRenderer->AddActor(extractedActor);  // 给右侧的渲染器添加Actor

	leftRenderer->ResetCamera();  // Automatically set up the camera based on the visible actors.
	// 也就是设置渲染器的Camera,让Actor位于位于camera的中央
	rightRenderer->ResetCamera(); //Automatically set up the camera based on the visible actors.
	//也就是开始更新vtk管线
	renderWindow->Render();  //  Ask each renderer owned by this RenderWindow to render its image and synchronize this process.
	interactor->Start();  // Start the event loop

	return 0;

}
