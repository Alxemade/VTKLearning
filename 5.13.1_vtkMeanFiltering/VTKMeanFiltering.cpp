#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageCast.h>
#include <vtkImageConvolve.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

/*
1. vtkImageConvolve可以实现简单的卷积操作
2. vtkImageGaussianSmooth可以实现高斯平滑操作
3. vtkImageHybridMedian2D可以实现中值滤波
4. vtkImageAnisotropicDiffusion2D实现各项异性扩散滤波
*/
int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("hua2.jpg");
	reader->Update();  // 首先读取一幅gray图像
	// 考虑到进行卷积运算时数据范围的变化和精度要求,我们将unsigned char类型转换成float类型
	vtkSmartPointer<vtkImageCast> originalCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	originalCastFilter->SetInputConnection(reader->GetOutputPort());
	originalCastFilter->SetOutputScalarTypeToFloat();  // 将unsigned char类型转换flaot类型
	originalCastFilter->Update();
	// 将图像和一个filter进行卷积运算
	vtkSmartPointer<vtkImageConvolve> convolveFilter =
		vtkSmartPointer<vtkImageConvolve>::New();  // Convolution of an image with a kernel.
	convolveFilter->SetInputConnection(originalCastFilter->GetOutputPort());
	double kernel[25] ={ 0.04,0.04,0.04,0.04,0.04,
						 0.04,0.04,0.04,0.04,0.04,
						 0.04,0.04,0.04,0.04,0.04,
						 0.04,0.04,0.04,0.04,0.04,
						 0.04,0.04,0.04,0.04,0.04 }; // 定义一个卷积核，但是要保证所有数值之和为1，这样就不用归一化了

	convolveFilter->SetKernel5x5(kernel);  // 设置5*5的卷积核和它卷积
	convolveFilter->Update();  // 更新

	vtkSmartPointer<vtkImageCast>convCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	convCastFilter->SetInputData(convolveFilter->GetOutput());
	convCastFilter->SetOutputScalarTypeToUnsignedChar();  // 然后再把输出的double类型数据在转换成unsigned char 类型
	convCastFilter->Update();

	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(convCastFilter->GetOutput());

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
	leftRenderer->SetBackground(.6, .5, .4);  // 设置左边的渲染器的背景颜色

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建右边的渲染器
	renderWindow->AddRenderer(rightRenderer); // 将右边渲染器添加到渲染窗口里面
	rightRenderer->SetViewport(rightViewport); // 设置渲染器的视窗
	rightRenderer->SetBackground(.4, .5, .6);  // 设置渲染器的背景

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
