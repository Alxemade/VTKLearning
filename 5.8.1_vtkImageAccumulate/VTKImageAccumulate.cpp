/* 注意本程序是针对灰度图的，如果是其他类型的图需要转换成gray图像*/

#include <vtkJPEGReader.h>
#include <vtkImageAccumulate.h>
#include <vtkSmartPointer.h>
#include <vtkIntArray.h>
#include <vtkDataObject.h>
#include <vtkImageData.h>
#include <vtkFieldData.h>
#include <vtkProperty2D.h>
#include <vtkLegendBoxActor.h>
#include <vtkBarChartActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("hua2.jpg");
	reader->Update();  // 读取一幅图像

	int bins = 16;
	vtkSmartPointer<vtkImageAccumulate> histogram =
		vtkSmartPointer<vtkImageAccumulate>::New();  // Generalized histograms up to 3 dimensions
	histogram->SetInputConnection(reader->GetOutputPort()); // 将读取reader连接起来
	histogram->SetComponentExtent(0, bins - 1, 0, 0, 0, 0); // 设置一维的直方图，这里设置的是16个组分
	histogram->SetComponentOrigin(0, 0, 0); //  Set/Get - The component origin is the location of bin (0, 0, 0).
	histogram->SetComponentSpacing(256 / bins, 0, 0);  // 设置每一个组分之间的间隔大小
	histogram->Update(); // 开始更新
	
	// 这里需要进行间接转换的原因：vtkImageAccumulate输出的类型是vtkImageData，这个是不能够直接利用直方图进行显
	// 所以我们需要借助vtkBarChartActor类进行转换，但是这个输入数据是vtkDataObject类型的，因此我们需要进行二步转换才可以
	vtkSmartPointer<vtkIntArray> frequencies =
		vtkSmartPointer<vtkIntArray>::New();  // dynamic, self-adjusting array of int,也就是动态数组的意思了
	frequencies->SetNumberOfComponents(1);  // ?
	frequencies->SetNumberOfTuples(bins);  // 动态数组，总共需要开辟turple
	
	int* output = static_cast<int*>(histogram->GetOutput()->GetScalarPointer());  // 将void *类型转换int *类型，类型转换
	// 将* vtkImageData类型的数据进行强制类型转换，最后转换的数据类型是int类型
	for (int j = 0; j < bins; ++j)
	{
		frequencies->SetTuple1(j, *output++);  // 将每一个组分相关的数据放置到这个frequencied里面
	}
	vtkSmartPointer<vtkDataObject> dataObject =
		vtkSmartPointer<vtkDataObject>::New();
	dataObject->GetFieldData()->AddArray(frequencies);  // Assign or retrieve a general field data to this data object.
	// 这里也就是首先将vtkImageData转换成vtkIntArray，然后在转换成vtkDataObject，最后可以输入到vtkChartActor显示
	vtkSmartPointer<vtkBarChartActor> barChart =
		vtkSmartPointer<vtkBarChartActor>::New();  // create a bar chart from an array

	barChart->SetInput(dataObject);
	barChart->SetTitle("Histogram");  // 设置标题
	barChart->GetPositionCoordinate()->SetValue(0.05, 0.05, 0.0);  // 设置barChart所在的位置坐标，这里设置的是左下角和右上角的坐标
	barChart->GetPosition2Coordinate()->SetValue(0.95, 0.95, 0.0);
	barChart->GetProperty()->SetColor(1, 1, 1);  // 设置颜色

	barChart->GetLegendActor()->SetNumberOfEntries(
		dataObject->GetFieldData()->GetArray(0)->GetNumberOfTuples());  // Specify the number of entries in the legend box.
	barChart->LegendVisibilityOff();  // 不打开图例
	barChart->LabelVisibilityOff(); // 不打开x轴的标签

	double red[3] = { 1, 0, 0 }; // 红色rgb刚好是1，0, 0
	for (int i = 0; i < bins; ++i)
	{
		barChart->SetBarColor(i, red); // 为每一个bin都设置成红色
	}

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(barChart);
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(640, 480);
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();

	return 0;

}

