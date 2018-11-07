#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

int main()
{
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->Render();
	std::cin.get();
	return 0;
}
