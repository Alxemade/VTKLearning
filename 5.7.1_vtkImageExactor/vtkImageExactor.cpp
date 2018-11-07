#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkExtractVOI.h> // 提取感兴趣的区域
#include <vtkImageCast.h>  // 强制类型转换

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
	extractVOI->Update();  // Bring this algorithm's outputs up-to-date,也就是实时更新我们的vtk管线了，这个输出就是实时的
	// 也就是说我们通过vtkExtractorVOI获得是提取感兴趣的区域了
	vtkImageData* extracted = extractVOI->GetOutput();  // Get the output data object for a port on this algorithm										// 然后我们在提取输出的区域
	int* extractedDims = extracted->GetDimensions();  // 得到我们VOI的维度
	std::cout << "Dims: " << " x: " << extractedDims[0]
		<< " y: " << extractedDims[1]
		<< " z: " << extractedDims[2] << std::endl;  // 输出VOI的维度
	std::cout << "Number of points: " << extracted->GetNumberOfPoints() << std::endl; // 输出VOI之后的点云的数目
	std::cout << "Number of cells: " << extracted->GetNumberOfCells() << std::endl;  // 输出VOI之后的数目组分的数目
	vtkSmartPointer<vtkImageCast> inputCastFilter =
		vtkSmartPointer<vtkImageCast>::New();  // vtkImageCast filter casts the input type to match the output type
	// vtkImageCast相当于是一个filter,对vtkImageData数据类型进行转换
	inputCastFilter->SetInputConnection(reader->GetOutputPort());  // 这个是cast是针对原始图像的
	inputCastFilter->SetOutputScalarTypeToUnsignedChar();  // 这个是将输出图像的类型转换成unsigned char了
	// 也就是说，这里的vtkImageCast只是为了更新将输入的图像都转换成unsigned char类型就可以了
	inputCastFilter->Update(); // 开始更新vtk管线

	vtkSmartPointer<vtkImageCast> extractedCastFilter =
		vtkSmartPointer<vtkImageCast>::New();  // 然后我们在建立一个vtkImageCast对象，转换的是提取VOI之后的数据
#if VTK_MAJOR_VERSION <= 5
	extractedCastFilter->SetInputConnection(extracted->GetProducerPort());  // 这个可能更新了版本就没有使用这个函数了
#else
	extractedCastFilter->SetInputData(extracted);  // SetInputData是没有建立一个管线连接
#endif
	extractedCastFilter->SetOutputScalarTypeToUnsignedChar(); // 这个将输入的数据变成unsigned char类型
	extractedCastFilter->Update();  // 更新vtk管线

	// Create actors，创建二个Actor，一个时输入的真实数据，另一个是VOI感兴趣的数据
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(inputCastFilter->GetOutput()); // 这个将这个经过cast转换成unsigned char数据连接到actor1

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(extractedCastFilter->GetOutput());  // 将这个VOI送到actor2
	// 如何实现在一个渲染窗口里面添加二副图像了？其实道理也是很简单的，就是使用一个渲染窗口，然后使用二个渲染器就可以了
	// There will be one render window
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 300);  // 设置渲染窗口的大小，600*300

	// And one interactor， 仅仅使用一个渲染窗口交互器
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	// Define viewport ranges
	// (xmin, ymin, xmax, ymax)， coordinate is 0 <= coordinate <= 1.0.范围是在[0-1]之间
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
