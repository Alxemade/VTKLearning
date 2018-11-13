#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageFFT.h>
#include <vtkImageRFFT.h>
#include <vtkImageCast.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main()
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ( "hua2.jpg" );  // 首先读取一幅图像
	reader->Update();  

	vtkSmartPointer<vtkImageFFT> fftFilter =
		vtkSmartPointer<vtkImageFFT>::New();  // Fast Fourier Transform.
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->Update();   // 将输入的图像进行快速fft变换，得到的图像是复数图像，是不能直接显示的，所以我们需要提取某一部分的图像
	// 下面的提取组分，然后变数据类型只是为了显示作用
	vtkSmartPointer<vtkImageExtractComponents> fftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();  // Outputs a single component
	fftExtractReal->SetInputConnection(fftFilter->GetOutputPort());
	fftExtractReal->SetComponents(0);  // 这里我们提取的实部图像
	// 因为vtkImageActor只能显示unsigned char类型的图像，所以我们需要将浮点型数据变成unsigned char类型
	vtkSmartPointer<vtkImageCast> fftCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	fftCastFilter->SetInputConnection(fftExtractReal->GetOutputPort());
	fftCastFilter->SetOutputScalarTypeToUnsignedChar();  // 将其变成unsigned char类型数据
	fftCastFilter->Update();
	// 如果直接想要进行傅里叶反变换的话，那么就直接输入频率域空间的数据
	vtkSmartPointer<vtkImageRFFT> rfftFilter =
		vtkSmartPointer<vtkImageRFFT>::New();  //  Reverse Fast Fourier Transform.
	rfftFilter->SetInputConnection(fftFilter->GetOutputPort()); 
	rfftFilter->Update();  // 将

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);  // 提取第一个组分

	vtkSmartPointer<vtkImageCast> rfftCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	rfftCastFilter->SetInputConnection(ifftExtractReal->GetOutputPort());
	rfftCastFilter->SetOutputScalarTypeToUnsignedChar();  // 将获取的数据变成Unsigned char类型
	rfftCastFilter->Update();

	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(reader->GetOutput());  // 显示的是原图像

	vtkSmartPointer<vtkImageActor> centerActor =
		vtkSmartPointer<vtkImageActor>::New();
	centerActor->SetInputData(fftCastFilter->GetOutput());  // 显示的图像的频率域信息

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(rfftCastFilter->GetOutput());

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

