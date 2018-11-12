#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageThreshold.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
	vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("hua2.jpg" );  // 首先读取一幅图像
	reader->Update();

	vtkSmartPointer<vtkImageThreshold> thresholdFilter =
		vtkSmartPointer<vtkImageThreshold>::New();  //  Flexible threshold,设置一个阈值将灰度图像变成2值图像

	thresholdFilter->SetInputConnection(reader->GetOutputPort());  // 这个也就相当于是一个filter
	thresholdFilter->ThresholdByUpper(100);  // 设置100为阈值，然后大于100为有效范围

	thresholdFilter->SetInValue(255); // 这里我们将100为有效阈值，大于100的设置为255
	thresholdFilter->SetOutValue(0);  // 低于100的我们设置为0
	thresholdFilter->Update();  // 更新

	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(reader->GetOutput()); 

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(thresholdFilter->GetOutput());  

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
	// 
	interactor->Start();  // Start the event loop.也就是开始启动

	return EXIT_SUCCESS;

}

