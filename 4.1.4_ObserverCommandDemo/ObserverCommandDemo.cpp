#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>

long pressCounts = 0;  // 全局变量，用于存储总共点击了多少次鼠标事件

//第一步，定义回调函数。
//注意回调函数的签名，我猜这里应该是形参的含义吧，不能更改。
void MyCallbackFunc(vtkObject*, unsigned long eid, void* clientdata, void *calldata)
{
	std::cout<<"You have clicked:"<<++pressCounts<<" times."<<std::endl;
}

int main(int argc, char*argv[])
{
	vtkSmartPointer<vtkBMPReader> reader =
		vtkSmartPointer<vtkBMPReader>::New();  // 首先声明一个bmpreader类
	reader->SetFileName("hua.bmp");
	reader->Allow8BitBMPOn();  // 打开8bit的窗口
	reader->SetDebug(0);  // 不打开Debug窗口
	reader->GlobalWarningDisplayOff();  // 关闭全局的警告显示信息
	reader->Update();  // 更新信息

	vtkSmartPointer<vtkImageViewer2> viewer =
		vtkSmartPointer<vtkImageViewer2>::New();  // 显示2d窗口一个比较方便的类
	viewer->SetInputData(reader->GetOutput());  // 这个vtkImageViewer2相当于一个简单啊vtkRenderWindow

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();  // Attach an interactor for the internal render window

	viewer->SetupInteractor(interactor);  // Attach an interactor for the internal render window.

	viewer->Render();   // Render the resulting image.

	//第二步，设置回调函数。

	vtkSmartPointer<vtkCallbackCommand> mouseCallback =
		vtkSmartPointer<vtkCallbackCommand>::New();  // 设置一个一个回调函数
	mouseCallback->SetCallback( MyCallbackFunc ); // 加入我们的回调函数，也就是说在命令行进行正确的显示

	 //第三步，将vtkCallbackCommand对象添加到观察者列表。

	interactor->SetRenderWindow(viewer->GetRenderWindow());  //  Set/Get the rendering window being controlled by this object.
	interactor->AddObserver(vtkCommand::LeftButtonPressEvent,mouseCallback);  // 添加观察者模式，添加vtk命令，也就是鼠标左键就触发事件
	interactor->Initialize();  // 初始化这个事件
	interactor->Start();  // 启动这个事件
	return 0;

}