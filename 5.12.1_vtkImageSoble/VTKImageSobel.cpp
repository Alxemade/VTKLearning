#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageSobel2D.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageMathematics.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
	vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName( "hua2.jpg" );
	reader->Update();  // 首先读取一幅灰度图像

	vtkSmartPointer<vtkImageSobel2D> sobelFilter =
		vtkSmartPointer<vtkImageSobel2D>::New(); // Computes a vector field using sobel functions.
	sobelFilter->SetInputConnection(reader->GetOutputPort());
	//  首先利用vtkImageSobel2D提取图像的梯度场，然后分别提取x分量和y分量
	vtkSmartPointer<vtkImageExtractComponents> extractXFilter =
	vtkSmartPointer<vtkImageExtractComponents>::New();  //  Outputs a single component
	extractXFilter->SetComponents(0);  // 提取x分量
	extractXFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractXFilter->Update();  // 计算每一个都需要立即更新一下

	double xRange[2];
	extractXFilter->GetOutput()->GetScalarRange(xRange);

	vtkSmartPointer<vtkImageMathematics> xImageAbs =
		vtkSmartPointer<vtkImageMathematics>::New();
	xImageAbs->SetOperationToAbsoluteValue(); // 由于提取的分量可能存在负值，所以首先需要进行计算它的绝对值
	xImageAbs->SetInputConnection(extractXFilter->GetOutputPort());
	xImageAbs->Update();

	vtkSmartPointer<vtkImageShiftScale> xShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();  // 由于取绝对值之后数值可能会超过255，所以我们需要调用vtkImageShiftScale归一化到0-255之间
	xShiftScale->SetOutputScalarTypeToUnsignedChar();
	xShiftScale->SetScale( 255 / xRange[1] );
	xShiftScale->SetInputConnection(xImageAbs->GetOutputPort());
	xShiftScale->Update();  // 还是需要立即更新一下

	vtkSmartPointer<vtkImageExtractComponents> extractYFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractYFilter->SetComponents(1); // 设置1表示提取y分量
	extractYFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractYFilter->Update();

	double yRange[2];
	extractYFilter->GetOutput()->GetScalarRange(yRange);
		vtkSmartPointer<vtkImageMathematics> yImageAbs =
	vtkSmartPointer<vtkImageMathematics>::New();
	yImageAbs->SetOperationToAbsoluteValue();
	yImageAbs->SetInputConnection(extractYFilter->GetOutputPort());
	yImageAbs->Update();

	vtkSmartPointer<vtkImageShiftScale> yShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	yShiftScale->SetOutputScalarTypeToUnsignedChar();
	yShiftScale->SetScale( 255 / yRange[1] );
	yShiftScale->SetInputConnection(yImageAbs->GetOutputPort());
	yShiftScale->Update();

	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> centerActor =
		vtkSmartPointer<vtkImageActor>::New();
	centerActor->SetInputData(xShiftScale->GetOutput());

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(yShiftScale->GetOutput());

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 300);  // 设置渲染窗口的大小，600*300

	// And one interactor， 仅仅使用一个渲染窗口交互器
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	double leftViewport[4] = { 0.0, 0.0, 0.33, 1.0 };
	double centerViewPort[4] = { 0.33, 0.0, 0.66, 1.0 };
	double rightViewport[4] = { 0.66, 0.0, 1.0, 1.0 };

	// Setup both renderers，虽然使用了一个渲染窗口，但是我们需要设置二个渲染器
	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建左边的渲染器
	renderWindow->AddRenderer(leftRenderer);  // 首先我们将我们左边的渲染器添加到渲染窗口里面去
	leftRenderer->SetViewport(leftViewport);  // 设置我们的左边渲染器的视窗
	leftRenderer->SetBackground(.6, .5, .4);  // 设置左边的渲染器的背景颜色

	vtkSmartPointer<vtkRenderer> centerRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建中间的渲染器
	renderWindow->AddRenderer(centerRenderer);  // 首先我们将我们中间的渲染器添加到渲染窗口里面去
	centerRenderer->SetViewport(centerViewPort);  // 设置我们的中间渲染器的视窗
	centerRenderer->SetBackground(.4, .6, .5);  // 设置中间的渲染器的背景颜色

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // 创建右边的渲染器
	renderWindow->AddRenderer(rightRenderer); // 将右边渲染器添加到渲染窗口里面
	rightRenderer->SetViewport(rightViewport); // 设置渲染器的视窗
	rightRenderer->SetBackground(.4, .5, .6);  // 设置渲染器的背景

	leftRenderer->AddActor(inputActor);  // 给左侧的渲染器添加Actor
	centerRenderer->AddActor(centerActor); // 给中间的渲染器添加Actor
	rightRenderer->AddActor(extractedActor);  // 给右侧的渲染器添加Actor

	leftRenderer->ResetCamera();  // Automatically set up the camera based on the visible actors.
	centerRenderer->ResetCamera();  
	rightRenderer->ResetCamera(); //Automatically set up the camera based on the visible actors.
	//也就是开始更新vtk管线
	renderWindow->Render();  //  Ask each renderer owned by this RenderWindow to render its image and synchronize this process.
	interactor->Start();  // Start the event loop

	return 0;

}
