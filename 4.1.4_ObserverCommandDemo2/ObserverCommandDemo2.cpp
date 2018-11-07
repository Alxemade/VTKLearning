#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkBoxWidget.h>
#include <vtkTransform.h>
#include <vtkInteractorStyleTrackballCamera.h>

//第一步
class vtkMyCallback : public vtkCommand  // 使用vtkMyCallback公有继承vtkCommand
{
public:
	static vtkMyCallback * New()  // 定义一个静态的成员函数
		{ return new vtkMyCallback; }  // 申请一块内存，建立一个vtkMyCallback对象
	// vtkCommand::Execure()函数定义为一个virtual void Execute() = 0的纯虚函数，这里相当于进行了override
	
	// caller: 第一个参数是caller，指向调用观察者的对象，即调用AddObserver()方法的那个对象
	// 第二个参数eventId是事件的编号。
	// 第三个参数callData，是传递给Execute函数的数据
	virtual void Execute(vtkObject *caller, unsigned long eventId, void* callData)
	{
		vtkTransform *t = vtkTransform::New();  // 首先创建一个vtkTransform对象
		vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);  // reinterpret_cast是将基类对象转换成派生类对象
		widget->GetTransform(t);  // 将这个vtkObject对象变换应用到vtkBoxWidget
		widget->GetProp3D()->SetUserTransform(t); 
		t->Delete();  // 删除指针
	}
};

int main()
{
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();   // 定义一个椎体的类
	cone->SetHeight( 3.0 );  // 设置椎体的高度
	cone->SetRadius( 1.0 );  // 设置椎体的半径
	cone->SetResolution( 10 );  // 设置椎体的分辨率
	vtkSmartPointer<vtkPolyDataMapper> coneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();  // 将这个椎体添加到Mapper里面
	coneMapper->SetInputConnection( cone->GetOutputPort() );

	vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();  // 定义一个Actor类
	coneActor->SetMapper(coneMapper );  // 设置这个Actor的几何图元

	vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();   //  设置一个渲染器
	ren1->AddActor(coneActor );  // 给这个渲染器添加Actor
	ren1->SetBackground(0.1, 0.2, 0.4 );  // 设置这个渲染器的背景颜色
	
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();  // 设置一个渲染窗口
	
	renWin->AddRenderer(ren1 );  // 给这个渲染窗口添加一个渲染器
	renWin->SetSize( 300,300 );  // 设置这个渲染窗口的大小
	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();  // 设置一个渲染窗口的交互器
	iren->SetRenderWindow(renWin);  // 给渲染窗口添加交互器

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // 设置交互器中相机的相关风格
	iren->SetInteractorStyle(style);  // 设置相关风格

	//通过vtkBoxWidget可以控制coneActor的变换矩阵，从而实现coneActor的形变
	vtkSmartPointer<vtkBoxWidget> boxWidget =vtkSmartPointer<vtkBoxWidget>::New();
	boxWidget->SetInteractor(iren);  
	boxWidget->SetPlaceFactor(1.25);
	boxWidget->SetProp3D(coneActor);
	boxWidget->PlaceWidget();  // 添加一个vtkBoxWidget类

	//第二步

	vtkSmartPointer<vtkMyCallback> callback = vtkSmartPointer<vtkMyCallback>::New();  // 实例化一个vtkMyCallback对象

	//第三步

	boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);  // 监听vtkBoxWidget对象的交互事件(InteractionEvent)。
	// 当用户与vtkBoxWidget对象交互时，事件InteractionEvent就会被触发，程序就会自动调用vtkMyCallback里的Execute()事件。
	//激活Widget。按“i”键可以关闭或激活Widget。
	boxWidget->On();  // 打开Widget

	iren->Initialize();
	iren->Start();

	return 0;
}
