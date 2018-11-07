#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkExtractVOI.h>
#include <vtkImageCast.h>

int main(int argn, char *argv[])
{
	// Create an image
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();  // 首先定义一个jpeg文件读取器
	reader->SetFileName("hua.jpg");  // 读取文件到reader里面啦
	reader->Update();  // 开始更新vtk管线了

	int* inputDims = reader->GetOutput()->GetDimensions();  // 读取这幅图像的维度，然后将这个保存到inputDims这个指针里面
	std::cout << "Dims: " << " x: " << inputDims[0]
		<< " y: " << inputDims[1]
		<< " z: " << inputDims[2] << std::endl;  // output the dimensions of the input image 
	std::cout << "Number of points: " << reader->GetOutput()->GetNumberOfPoints() << std::endl;  // 获取这幅图像的点的个数
	std::cout << "Number of cells: " << reader->GetOutput()->GetNumberOfCells() << std::endl;  // 获取这幅图像的组分的个数

	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();  // vtkExtractorVOI是提取我们感兴趣的区域的类，这个就相当于是一个filter
	extractVOI->SetInputConnection(reader->GetOutputPort());  // 然后我们将我们reader读取图像的类和这个filter连接起来
	extractVOI->SetVOI(inputDims[0] / 4., 3.*inputDims[0] / 4., inputDims[1] / 4., 3.*inputDims[1] / 4., 0, 0); // Specify i-j-k (min,max) pairs to extract
	extractVOI->Update();

	vtkImageData* extracted = extractVOI->GetOutput();

	int* extractedDims = extracted->GetDimensions();
	std::cout << "Dims: " << " x: " << extractedDims[0]
		<< " y: " << extractedDims[1]
		<< " z: " << extractedDims[2] << std::endl;
	std::cout << "Number of points: " << extracted->GetNumberOfPoints() << std::endl;
	std::cout << "Number of cells: " << extracted->GetNumberOfCells() << std::endl;

	vtkSmartPointer<vtkImageCast> inputCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	inputCastFilter->SetInputConnection(reader->GetOutputPort());
	inputCastFilter->SetOutputScalarTypeToUnsignedChar();
	inputCastFilter->Update();

	vtkSmartPointer<vtkImageCast> extractedCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
#if VTK_MAJOR_VERSION <= 5
	extractedCastFilter->SetInputConnection(extracted->GetProducerPort());
#else
	extractedCastFilter->SetInputData(extracted);
#endif
	extractedCastFilter->SetOutputScalarTypeToUnsignedChar();
	extractedCastFilter->Update();

	// Create actors
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(inputCastFilter->GetOutput());

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(extractedCastFilter->GetOutput());

	// There will be one render window
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 300);

	// And one interactor
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	// Define viewport ranges
	// (xmin, ymin, xmax, ymax)
	double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

	// Setup both renderers
	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(leftRenderer);
	leftRenderer->SetViewport(leftViewport);
	leftRenderer->SetBackground(.6, .5, .4);

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(rightRenderer);
	rightRenderer->SetViewport(rightViewport);
	rightRenderer->SetBackground(.4, .5, .6);

	leftRenderer->AddActor(inputActor);
	rightRenderer->AddActor(extractedActor);

	leftRenderer->ResetCamera();

	rightRenderer->ResetCamera();

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}
