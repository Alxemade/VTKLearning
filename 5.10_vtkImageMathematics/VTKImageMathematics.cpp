#include <vtkSmartPointer.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageMathematics.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkImageCanvasSource2D>imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();  // Paints on a canvas,也就是创建一个黑色的幕布，然后在上面画点
	imageSource->SetNumberOfScalarComponents(3);  // 设置三维
	imageSource->SetScalarTypeToUnsignedChar();  // 将类型设置成unsigned char类型
	imageSource->SetExtent(0, 4, 0, 4, 0, 0);  // 设置输出的范围，注意当前只是幕布的范围
	imageSource->SetDrawColor(100.0, 0, 0);  //画颜色，这个是在幕布上画出颜色
	imageSource->FillBox(0, 4,0, 4);  // 开始在幕布上填充
	imageSource->Update();  // 更新
	
	vtkSmartPointer<vtkImageMathematics>imageMath =
		vtkSmartPointer<vtkImageMathematics>::New(); //  vtkImageMathematics implements basic mathematic operations
	// vtkImageMathematics提供了基本的一元和二元数学操作
	imageMath->SetOperationToMultiplyByK(); // Set each pixel in the output image to the product of ConstantK with the
	imageMath->SetConstantK(2.0);  // 乘积的常数是2
	imageMath->SetInputConnection(imageSource->GetOutputPort());  // 然后连接这个管线，更新管线
	imageMath->Update();
	// 下面就是显示的一些东西
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(imageSource->GetOutput()); 

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(imageMath->GetOutput());  

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

