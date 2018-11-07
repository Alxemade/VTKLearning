#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>

int main(int argc, char*argv[])
{

	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("lena.bmp");
	reader->Allow8BitBMPOn();  // 打开bmp的8bit的开关，VTK不支持单色的BMP图像的读取

	reader->SetDebug(1);  // 等同于DebugOn()
	//reader->SetDebug(0);  // 等同于等同于DebugOff()
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