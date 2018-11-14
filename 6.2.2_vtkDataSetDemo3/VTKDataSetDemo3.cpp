#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>

int main(int argc, char*argv[])
{
	//�������������
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 1.0, 0.0, 0.0 ); //���ص�һ�����ID��0
	points->InsertNextPoint( 0.0, 0.0, 1.0 ); //���صڶ������ID��1
	points->InsertNextPoint( 0.0, 0.0, 0.0 ); //���ص��������ID��2

	//ÿ���������֮��ֱ𴴽�һ����
	//SetId()�ĵ�һ���������߶εĶ˵�ID���ڶ������������ӵĵ��ID
	vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId ( 0, 0 );  // Set the id at location i
	line0->GetPointIds()->SetId ( 1, 1 );

	vtkSmartPointer<vtkLine>line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId ( 0, 1 );
	line1->GetPointIds()->SetId ( 1, 2 );

	vtkSmartPointer<vtkLine> line2 =vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId( 0, 2 );
	line2->GetPointIds()->SetId ( 1, 0 );

	//������Ԫ���飬���ڴ洢���ϴ������߶�
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell (line0 );
	lines->InsertNextCell (line1 );
	lines->InsertNextCell (line2 );

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

	//������߼��뵽���ݼ��У�ǰ�߶������ݼ��ļ��νṹ�����߶������˽ṹ
	polydata->SetPoints (points );  // ���弸�νṹ
	polydata->SetLines (lines );  // �������˽ṹ

	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName ("TriangleLines.vtk" );
	writer->SetInputData(polydata );
	writer->Write();

	return 0;

}
