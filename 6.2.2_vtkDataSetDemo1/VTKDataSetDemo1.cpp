#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>

int main(int argc, char *argv[])
{
	//����������
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint ( 1.0, 0.0, 0.0 );  // ��������������
	points->InsertNextPoint ( 0.0, 0.0, 0.0 );
	points->InsertNextPoint ( 0.0, 1.0, 0.0 );

	//����vtkPolyData���͵����ݣ�vtkPolyData������vtkPointSet��
	//vtkPointSet��vtkDataSet�����ࡣ
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

	//�������ĵ����ݼ��뵽vtkPolyData������
	polydata->SetPoints ( points );  // Specify point array to define point coordinates.

	//��vtkPolyData���͵�����д�뵽һ��vtk�ļ�������λ���ǹ��̵�ǰĿ¼
	vtkSmartPointer< vtkPolyDataWriter > writer = vtkSmartPointer<vtkPolyDataWriter >::New();  //  write vtk polygonal data
	writer->SetFileName("triangle.vtk");  // ���ñ�����ļ�����.vtk
	writer->SetInputData(polydata);  // д������
	writer->Write();  // Write data to output

	return 0;

}
