#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>  // ����vtkActor�������
#include <vtkTexture.h>   // ����ͼƬ������
#include <vtkJPEGReader.h>  // ��ȡjpeg�ļ�
#include <vtkLight.h>  //  vtkSetMacro(Positional, int); ��ʵ��ʹ�ú���ж�����صĺ�����
#include <vtkCamera.h>

int main()
{
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
	jpegReader->SetFileName("hua.jpg");

	vtkSmartPointer<vtkTexture> cylinderTexture = vtkSmartPointer<vtkTexture>::New();
	cylinderTexture->SetInputConnection(jpegReader->GetOutputPort());
	cylinderTexture->InterpolateOn();  // Turn on linear interpolation of the texture map when rendering.

	vtkSmartPointer<vtkCylinderSource> cylinder =
		vtkSmartPointer<vtkCylinderSource>::New();  // vtkSmartPointer������ָ�롣vtkCylinderSourece��һ����̬��Ա����New()
	cylinder->SetHeight( 3.0 );  // ����Բ����ĸ߶�
	cylinder->SetRadius( 1.0 );  // ����Բ����İ뾶
	cylinder->SetResolution( 10 );  // ����Բ����ķֱ���
	
	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();  // ��Ⱦ����μ�������
	cylinderMapper->SetInputConnection( cylinder->GetOutputPort() );  //  VTK���ӻ����ߵ��������ݽӿ�

	vtkSmartPointer<vtkActor> cylinderActor =
		vtkSmartPointer<vtkActor>::New();  // ��Ⱦ���������ݵĿ��ӻ������ͨ��vtkProp�����ฺ��ġ�
	cylinderActor->SetMapper( cylinderMapper );  // �������ɼ���ͼԪ��Mapper��������һ��Actor�����ӻ����ߵ�ĩ��(���ӻ����ߵ�ĩ�˾���Mapper)
	// �ı�Actor������Ϊ��ɫ����һ�ַ�����ֱ�ӵ���SetColor�����Ϳ�����
	cylinderActor->GetProperty()->SetColor(1.0, 1.0, 1.0);  // �������ǵ���ɫΪ��ɫ
	// 2. ����Actor���Եķ���������ʵ����vtkProperty����Ȼ����뵽Actor�

	/*vtkSmartPointer<vtkProperty> cylinderProperty = vtkSmartPointer<vtkProperty>::New();
	cylinderProperty->SetColor(1.0, 0.0, 0.0);
	cylinderActor->SetProperty(cylinderProperty);*/

	// ��������
	cylinderActor->SetTexture(cylinderTexture);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();  // �������������Ⱦ����
	renderer->AddActor( cylinderActor );   // ���vtkProp���͵Ķ�����Ⱦ������
	renderer->SetBackground( 0.1, 0.2, 0.4 );  // ����������Ⱦ�����ı�����ɫ

	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();  // ������ϵͳ��VTK��Ⱦ�������ӵ�һ��,��ͬƽ̨�µ�vtkRenderWindow���ฺ�𱾵ؼ����ϵͳ�д��ڴ�������Ⱦ���̹���
	renWin->AddRenderer( renderer );  // ����vtkRenderer����
	renWin->SetSize( 300, 300 );  // ���ô��ڵĴ�С��������Ϊ��λ��
	
	// ������صĹ���
	vtkSmartPointer<vtkLight> myLight1 = vtkSmartPointer<vtkLight>::New();  // ���ù���ѡ��
	myLight1->SetColor(0, 1, 0);  // ���ù����ɫΪ��ɫ
	myLight1->SetPosition(0, 0, 1);  // ���ù�Դ��λ��
	myLight1->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());  // ���ù�Դ�Ľ���
	renderer->AddLight(myLight1);  // ��vtkRender����ӹ�Դ����Ϊ������Ӻܶ���գ����Ժ�������AddLight�ˣ�������SetLight��

	vtkSmartPointer<vtkLight> myLight2 = vtkSmartPointer<vtkLight>::New();  // ���ù���ѡ��
	myLight2->SetColor(0, 0, 1);  // ���ù����ɫΪ��ɫ
	myLight2->SetPosition(0, 0, -1);  // ���ù�Դ��λ��
	myLight2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());  // ���ù�Դ�Ľ���
	renderer->AddLight(myLight2);  // ��vtkRender����ӹ�Դ

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();  // �ṩƽ̨��������Ӧ��ꡢ���̺�ʱ���¼��Ľ�������
	iren->SetRenderWindow(renWin);  // ������Ⱦ���ڣ���Ϣ��ͨ����Ⱦ���ڲ��񵽵ģ����Ա���Ҫ������������������Ⱦ���ڡ�
	
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // ��������ʽ��һ�֣�����ʽ�£��û���ͨ�������������������ת���Ŵ���С�Ȳ�����
	iren->SetInteractorStyle(style);   // ���彻������ʽ��Ĭ�ϵĽ�����ʽΪvtkInteractorStyleSwitch��

	iren->Initialize();  // Ϊ�������¼���׼��������������֮ǰ�����ȵ�������������г�ʼ����
	iren->Start();  // ����ʼ�����¼���Ӧѭ�������������ڵȴ�״̬���ȴ��û������¼��ķ�����

	return 0;
}
