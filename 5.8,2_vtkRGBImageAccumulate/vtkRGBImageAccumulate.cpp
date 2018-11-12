/* 注意本程序是针对RGB，因为rgb不能直接计算灰度直方图，所以我们需要依次提取每一个通道的数值，在进行后续处理*/

#include <vtkJPEGReader.h>
#include <vtkXYPlotActor.h>
#include <vtkImageExtractComponents.h>
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
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ( "hua.jpg" );
	reader->Update(); // 读取一幅rgb图像

	int numComponents = reader->GetOutput()->GetNumberOfScalarComponents(); // 这个是获取图像是几通道的，这里是3通道的RGB
	//std::cout << "num of components: "<< numComponents << std::endl;
	vtkSmartPointer<vtkXYPlotActor> plot =
		vtkSmartPointer<vtkXYPlotActor>::New();  //  generate an x-y plot from input dataset(s) or field data
	// 上面我们使用gray是柱状图，这里的话我们相当于使用是折线图
	plot->ExchangeAxesOff();  // 不交换x,y轴
	plot->SetLabelFormat( "%g" );  // 设置标签的格式是%g
	plot->SetXTitle( "Value" );  // 设置x轴的标签
	plot->SetYTitle( "Frequency" ); // 设置y轴的标签
	plot->SetXValuesToValue(); // 设置x坐标是索引，数值还是其他，这里设置的是就是按照数值进行计算

	double colors[3][3] = {
							{ 1.0, 0.0, 0.0 },  // 设置显示vtkXYPlotActor绘画的颜色，分别是红色，绿色和蓝色
							{ 0.0, 1.0, 0.0 },
							{ 0.0, 0.0, 1.0 }
	};
	const char* labels[3] = { "Red", "Green","Blue" };  // 设置plot的标签

	int xmax = 0;  // 存放三通道最大的x
	int ymax = 0;  // 存放三通道最大的y

	for( int i = 0; i < numComponents; ++i )  // 分别循环rgb的三个通道
	{
		vtkSmartPointer<vtkImageExtractComponents> extract =
			vtkSmartPointer<vtkImageExtractComponents>::New(); // Outputs a single component
		extract->SetInputConnection( reader->GetOutputPort() );
		extract->SetComponents( i );  // 设置当前提取的通道
		extract->Update();

		double range[2];
		extract->GetOutput()->GetScalarRange( range );  // 提取某一个通道的范围
		int extent =static_cast<int>(range[1])-static_cast<int>(range[0])-1;
		// 这里的extent是将最大的像素值-最小的灰度值，而不是从0开始的，而是从当前的最小的灰度值开始的
		vtkSmartPointer<vtkImageAccumulate> histogram =
			vtkSmartPointer<vtkImageAccumulate>::New();
		
		histogram->SetInputConnection( extract->GetOutputPort() );
		histogram->SetComponentExtent( 0,extent, 0,0, 0,0);
		histogram->SetComponentOrigin( range[0],0,0 ); // 这里注意第一个参数是变化的，从最小的像素值开始，而并不是一定是0
		histogram->SetComponentSpacing( 1,0,0 );  // 间隔1
		histogram->SetIgnoreZero( 1 );
		histogram->Update();
		// 通过上面的GetScalarRange我们可以得到x的像素值范围
		if( range[1] > xmax )
		{
			xmax = range[1];  // 赋值给最大的xmax
		}
		if( histogram->GetOutput()->GetScalarRange()[1] > ymax )  // 利用GetScalarRange()[1]获取灰度图的最大值
		{
			ymax =histogram->GetOutput()->GetScalarRange()[1];
		}
		// 这里和灰度图那个实例不太一样，那里histogram是直接可以作为vtkXYPlotActor输入变量的
		plot->AddDataSetInput( histogram->GetOutput() );  // 将上面的histogram连接起来
		plot->SetPlotColor(i,colors[i]);  // 设置颜色
		plot->SetPlotLabel(i,labels[i]);  // 设置标签
		plot->LegendOn();  // 颜色开关打开
	}

	plot->SetXRange( 0, xmax);  // 设置三个通道中最大的x
	plot->SetYRange( 0, ymax);  // 设置三个通道中最大的y

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(plot);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize(640, 480);
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );
	interactor->Initialize();
	interactor->Start();

	return 0;

}