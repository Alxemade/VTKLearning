#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>

int main(int argc, char*argv[])
{
	 //�����������
	double X[3] = {1.0, 0.0, 0.0};
	double Y[3] = {0.0, 0.0, 1.0};
	double Z[3] = {0.0, 0.0, 0.0};

	//�����������Լ���ÿ���������ϼ��붥��(Vertex)���͵ĵ�Ԫl
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();  // represent and manipulate 3D points
	vtkSmartPointer<vtkCellArray> vertices =vtkSmartPointer<vtkCellArray>::New();  // object to represent cell connectivity

	for (unsigned int i = 0;i < 3; ++i)
	{
		//�������ڴ洢���������м������vtkIdType���൱��int��long������
		vtkIdType pid[1];

		//��ÿ����������뵽vtkPoints�У�InsertNextPoint()���ؼ���ĵ�������ţ�
		//����������Ҫʹ����������������������͵ĵ�Ԫ
		pid[0] = points->InsertNextPoint ( X[i], Y[i], Z[i] );  

		//��ÿ��������Ϸֱ𴴽�һ�����㣬�����ǵ�Ԫ(Cell)���һ������
		vertices->InsertNextCell(1, pid );
	}

	//����vtkPolyData����
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

	//ָ�����ݼ��ļ��νṹ(��pointsָ��)���Լ����ݼ������˽ṹ(��verticesָ��)
	polydata->SetPoints (points );
	polydata->SetVerts (vertices );

	//�����ɵ����ݼ�д��TriangleVerts.vtk�ļ�������ڹ��̵�ǰĿ¼��
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName ("TriangleVerts.vtk" );
	writer->SetInputData(polydata );
	writer->Write();
	return 0;
}
