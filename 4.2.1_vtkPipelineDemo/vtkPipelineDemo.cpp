#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

int main(int argc, char *argv[])
{
	//读入Structured_Points类型的vtk文件。
	vtkSmartPointer<vtkStructuredPointsReader> reader =
		vtkSmartPointer<vtkStructuredPointsReader>::New();  // 新建一个vtk结构的读取器
	reader->SetFileName("CT-Head.vtk");  // 读取vtk文件

	//用移动立方体法提取等值面。
	vtkSmartPointer<vtkMarchingCubes> marchingCubes =
		vtkSmartPointer<vtkMarchingCubes>::New(); // 新建一个MarchingCubes类
	marchingCubes->SetInputConnection(reader->GetOutputPort());
	marchingCubes->SetValue(0,500); // Set a particular contour value at contour number i

	//将生成的等值面数据进行Mapper
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(marchingCubes->GetOutputPort());

	//把Mapper的输出送入渲染引擎进行显示

	//////////////////////////////////////渲染引擎部分////////////////////////////////////
	vtkSmartPointer<vtkActor> actor =vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renWin->AddRenderer(renderer);  // 将渲染器添加到渲染窗口里面

	interactor->SetRenderWindow(renWin);  // 将交互器添加到渲染窗口里面
	renderer->AddActor(actor);  // 在渲染器中添加actor
	renderer->Render();  // ?这个作用是什么？
	// 一般来说，vtk采取了命令驱动的方式处理vtk管线连接，也就是说当我们的数据对象发生改变的时候，并不是马上做出改变，
	// 而是等待请求。比如Updata()函数，但是一般不这么做，而是使用renderer->Render()对整体发出数据请求，一级一级开始update了。
	interactor->Initialize(); // 交互器初始化
	interactor->Start();  // 交互器开始启动
	//////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
 }
