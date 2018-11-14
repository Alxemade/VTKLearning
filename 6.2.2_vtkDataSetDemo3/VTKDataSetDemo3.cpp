#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>

int main(int argc, char*argv[])
{
	//创建三个坐标点
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 1.0, 0.0, 0.0 ); //返回第一个点的ID：0
	points->InsertNextPoint( 0.0, 0.0, 1.0 ); //返回第二个点的ID：1
	points->InsertNextPoint( 0.0, 0.0, 0.0 ); //返回第三个点的ID：2

	//每两个坐标点之间分别创建一条线
	//SetId()的第一个参数是线段的端点ID，第二个参数是连接的点的ID
	vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId ( 0, 0 );  // Set the id at location i
	line0->GetPointIds()->SetId ( 1, 1 );

	vtkSmartPointer<vtkLine>line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId ( 0, 1 );
	line1->GetPointIds()->SetId ( 1, 2 );

	vtkSmartPointer<vtkLine> line2 =vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId( 0, 2 );
	line2->GetPointIds()->SetId ( 1, 0 );

	//创建单元数组，用于存储以上创建的线段
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell (line0 );
	lines->InsertNextCell (line1 );
	lines->InsertNextCell (line2 );

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

	//将点和线加入到数据集中，前者定义数据集的几何结构，后者定义拓扑结构
	polydata->SetPoints (points );  // 定义几何结构
	polydata->SetLines (lines );  // 定义拓扑结构

	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName ("TriangleLines.vtk" );
	writer->SetInputData(polydata );
	writer->Write();

	return 0;

}
