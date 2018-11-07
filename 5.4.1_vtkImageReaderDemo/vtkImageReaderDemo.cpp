#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>  // 2D图像查看器,包含Actor，RenderWindow, Renderer, RenderWindowInteractorStyle 
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
// GetDimensions(), GetOrigin(), GetSpacing() 这些函数都是vtkImageData的成员函数
int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader = 
		vtkSmartPointer<vtkJPEGReader>::New();  // 首先定义一个vtk中JPG图像读取器
	reader->SetFileName ( "hua.jpg");   // 读取的文件名
	reader->Update();   // 更新vtk管线, Bring this algorithm's outputs up-to-date.
	// GetOutput()一般的输出都是vtkImageData,包含二维的也包含三维的
	int dims[3]; // 这里是存储图像的维度
	reader->GetOutput()->GetDimensions(dims);  // GetDimensions, Get dimensions of this structured points dataset.
	// 使用GetDimensions维度之后我们再将数据存放到dims数组里面
	std::cout<<"The dimension of the image: "<<dims[0]<<" "<<dims[1]<<" "<<dims[2]<<std::endl;  // 输入该图像的各个维度

	double origin[3]; // 因为这里存储的是物体坐标，所以我们使用double这个数据类型进行存储
	reader->GetOutput()->GetOrigin(origin);  // Set/Get the origin of the dataset，也就是获取原点
	std::cout<<"The origin of the image: "<<origin[0]<<" "<<origin[1]<<" "<<origin[2]<<std::endl;

	double spaceing[3]; // 存储图像像素的组分，也就是像素之间的物理间隔
	reader->GetOutput()->GetSpacing(spaceing);  // 获取图像的组分
	std::cout<<"The piexl space of the image: "<<spaceing[0]<<" "<<spaceing[1]<<" "<<spaceing[2]<<std::endl;

	vtkSmartPointer<vtkImageViewer2>imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();  // 定义一个二维的vtkImageView2类

	imageViewer->SetInputConnection(reader->GetOutputPort());  // 建立渲染引擎和vtk管线之间的连接
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();  // 建立窗口和交互器之间的连接
	
	imageViewer->SetupInteractor(renderWindowInteractor);  // 将交互器添加到vtkImageView2
	imageViewer->Render();  // 更新vtk管线
	imageViewer->GetRenderer()->ResetCamera(); // Automatically set up the camera based on the visible actors.
	// 这个主要目的就是将照相机放到Renderer中央，这样我们就可以完全看到这个Actor对象了
	imageViewer->Render();  // 更新vtk管线

	renderWindowInteractor->Start();  // 开启交互器

	return 0;

}
