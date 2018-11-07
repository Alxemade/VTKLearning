#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>

int main(int argc, char*argv[])
{

	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("lena.bmp");
	reader->Allow8BitBMPOn();  // ��bmp��8bit�Ŀ��أ�VTK��֧�ֵ�ɫ��BMPͼ��Ķ�ȡ

	reader->SetDebug(1);  // ��ͬ��DebugOn()
	//reader->SetDebug(0);  // ��ͬ�ڵ�ͬ��DebugOff()
	//reader->GlobalWarningDisplayOff();
	reader->Update();
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	
	viewer->SetInputData(reader->GetOutput());
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	
	viewer->SetupInteractor(interactor);
	viewer->Render();
	
	interactor->Initialize();
	interactor->Start();

	return 0;

}