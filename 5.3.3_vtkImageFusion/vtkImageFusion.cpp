#include <vtkSmartPointer.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkJPEGReader.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();  // 首先读取一幅图像的类
	reader->SetFileName ("hua.jpg" );  // 然后开始读取一幅图像
	reader->Update();  // 开始可视化管线，然后进行更新

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();  //  vtkImageCanvasSource2D is a source that starts as a blank image
	imageSource->SetNumberOfScalarComponents(1); // 设置归一化的组分为1
	imageSource->SetScalarTypeToUnsignedChar();  // 设置归一化的尺度的类型为unsigned char类型
	imageSource->SetExtent(0, 512, 0, 512, 0, 0);  // set the WholeExtent of the output设置输出的全部延伸
	imageSource->SetDrawColor(0.0);  // Set DrawColor to (a, 0, 0, 0)
	imageSource->FillBox(0, 512, 0, 512);  //  Set the pixels inside the box (min0, max0, min1, max1) to the current
	imageSource->SetDrawColor(255.0);  // Set  DrawColor  to (a, 0，0，0）
	imageSource->FillBox(100,400,100,400);  // 在绘制一个正方形盒子
	imageSource->Update();  // 更新可视化管线

	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();  // blend images together using alpha or opacity
	imageBlend->AddInputData(0, reader->GetOutput());
	imageBlend->AddInputData(1, imageSource->GetOutput());
	imageBlend->SetOpacity(0, 0.4); // 给端口0设置不透明度0.4
	imageBlend->SetOpacity(1, 0.6); //  给端口1设置不透明为0.6
	imageBlend->Update();  // 更新vtkImageBlend
	return 0;
}

