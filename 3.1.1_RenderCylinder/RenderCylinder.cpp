#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>  // 设置vtkActor相关属性
#include <vtkTexture.h>   // 设置图片的纹理
#include <vtkJPEGReader.h>  // 读取jpeg文件
#include <vtkLight.h>  //  vtkSetMacro(Positional, int); 其实是使用宏进行定义相关的函数的
#include <vtkCamera.h>

int main()
{
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
	jpegReader->SetFileName("hua.jpg");

	vtkSmartPointer<vtkTexture> cylinderTexture = vtkSmartPointer<vtkTexture>::New();
	cylinderTexture->SetInputConnection(jpegReader->GetOutputPort());
	cylinderTexture->InterpolateOn();  // Turn on linear interpolation of the texture map when rendering.

	vtkSmartPointer<vtkCylinderSource> cylinder =
		vtkSmartPointer<vtkCylinderSource>::New();  // vtkSmartPointer是智能指针。vtkCylinderSourece有一个静态成员函数New()
	cylinder->SetHeight( 3.0 );  // 设置圆柱体的高度
	cylinder->SetRadius( 1.0 );  // 设置圆柱体的半径
	cylinder->SetResolution( 10 );  // 设置圆柱体的分辨率
	
	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();  // 渲染多边形几何数据
	cylinderMapper->SetInputConnection( cylinder->GetOutputPort() );  //  VTK可视化管线的输入数据接口

	vtkSmartPointer<vtkActor> cylinderActor =
		vtkSmartPointer<vtkActor>::New();  // 渲染场景中数据的可视化表达是通过vtkProp的子类负责的。
	cylinderActor->SetMapper( cylinderMapper );  // 设置生成几何图元的Mapper。即连接一个Actor到可视化管线的末端(可视化管线的末端就是Mapper)
	// 改变Actor的属性为红色，第一种方法：直接调用SetColor函数就可以了
	cylinderActor->GetProperty()->SetColor(1.0, 1.0, 1.0);  // 设置我们的颜色为红色
	// 2. 控制Actor属性的方法二：先实例化vtkProperty对象，然后加入到Actor里。

	/*vtkSmartPointer<vtkProperty> cylinderProperty = vtkSmartPointer<vtkProperty>::New();
	cylinderProperty->SetColor(1.0, 0.0, 0.0);
	cylinderActor->SetProperty(cylinderProperty);*/

	// 设置纹理
	cylinderActor->SetTexture(cylinderTexture);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();  // 负责管理场景的渲染过程
	renderer->AddActor( cylinderActor );   // 添加vtkProp类型的对象到渲染场景中
	renderer->SetBackground( 0.1, 0.2, 0.4 );  // 用于设置渲染场景的背景颜色

	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();  // 将操作系统与VTK渲染引擎连接到一起,不同平台下的vtkRenderWindow子类负责本地计算机系统中窗口创建和渲染过程管理。
	renWin->AddRenderer( renderer );  // 加入vtkRenderer对象。
	renWin->SetSize( 300, 300 );  // 设置窗口的大小，以像素为单位。
	
	// 设置相关的光照
	vtkSmartPointer<vtkLight> myLight1 = vtkSmartPointer<vtkLight>::New();  // 设置光照选项
	myLight1->SetColor(0, 1, 0);  // 设置光的颜色为绿色
	myLight1->SetPosition(0, 0, 1);  // 设置光源的位置
	myLight1->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());  // 设置光源的焦点
	renderer->AddLight(myLight1);  // 在vtkRender中添加光源，因为可以添加很多光照，所以函数就是AddLight了，而不是SetLight了

	vtkSmartPointer<vtkLight> myLight2 = vtkSmartPointer<vtkLight>::New();  // 设置光照选项
	myLight2->SetColor(0, 0, 1);  // 设置光的颜色为蓝色
	myLight2->SetPosition(0, 0, -1);  // 设置光源的位置
	myLight2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());  // 设置光源的焦点
	renderer->AddLight(myLight2);  // 在vtkRender中添加光源

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();  // 提供平台独立的响应鼠标、键盘和时钟事件的交互机制
	iren->SetRenderWindow(renWin);  // 设置渲染窗口，消息是通过渲染窗口捕获到的，所以必须要给交互器对象设置渲染窗口。
	
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // 交互器样式的一种，该样式下，用户是通过控制相机对物体作旋转、放大、缩小等操作。
	iren->SetInteractorStyle(style);   // 定义交互器样式，默认的交互样式为vtkInteractorStyleSwitch。

	iren->Initialize();  // 为处理窗口事件做准备，交互器工作之前必须先调用这个方法进行初始化。
	iren->Start();  // —开始进入事件响应循环，交互器处于等待状态，等待用户交互事件的发生。

	return 0;
}
