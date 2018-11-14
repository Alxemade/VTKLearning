#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>

int main(int argc, char *argv[])
{
	//创建点数据
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint ( 1.0, 0.0, 0.0 );  // 创建三个点数据
	points->InsertNextPoint ( 0.0, 0.0, 0.0 );
	points->InsertNextPoint ( 0.0, 1.0, 0.0 );

	//创建vtkPolyData类型的数据，vtkPolyData派生自vtkPointSet，
	//vtkPointSet是vtkDataSet的子类。
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

	//将创建的点数据加入到vtkPolyData数据里
	polydata->SetPoints ( points );  // Specify point array to define point coordinates.

	//将vtkPolyData类型的数据写入到一个vtk文件，保存位置是工程当前目录
	vtkSmartPointer< vtkPolyDataWriter > writer = vtkSmartPointer<vtkPolyDataWriter >::New();  //  write vtk polygonal data
	writer->SetFileName("triangle.vtk");  // 设置保存的文件名是.vtk
	writer->SetInputData(polydata);  // 写入数据
	writer->Write();  // Write data to output

	return 0;

}
