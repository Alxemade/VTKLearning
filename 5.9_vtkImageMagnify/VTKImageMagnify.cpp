#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageShrink3D.h>
#include <vtkImageMagnify.h>
#include <vtkImageData.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName (" hua.jpg" );
	reader->Update();  // 首先读取一幅图像
	 
	vtkSmartPointer<vtkImageShrink3D> shrinkFilter =
		vtkSmartPointer<vtkImageShrink3D>::New(); // 利用vtkImageShrink3D进行降采样
	shrinkFilter->SetInputConnection(reader->GetOutputPort());
	shrinkFilter->SetShrinkFactors(20,20,1);  // 对x,y,z三个方向分别降采样的因子
	shrinkFilter->Update();

	vtkSmartPointer<vtkImageMagnify> magnifyFilter =
		vtkSmartPointer<vtkImageMagnify>::New();  // 利用vtkImageMagnify进行升采样
	magnifyFilter->SetInputConnection(reader->GetOutputPort());
	magnifyFilter->SetMagnificationFactors(10,10,1);  // 设置升采样的因子
	magnifyFilter->Update();

	int originalDims[3];  // 原图像的维度
	reader->GetOutput()->GetDimensions(originalDims);

	double originalSpace[3];  // 原图像的间隔
	reader->GetOutput()->GetSpacing(originalSpace);

	int shrinkDims[3];  // 降采样
	shrinkFilter->GetOutput()->GetDimensions(shrinkDims);

	double shrinkSpace[3];
	shrinkFilter->GetOutput()->GetSpacing(shrinkSpace);

	int magnifyDims[3];  // 升采样
	magnifyFilter->GetOutput()->GetDimensions(magnifyDims);

	double magnifySpace[3];
	magnifyFilter->GetOutput()->GetSpacing(magnifySpace);
	// 保持dimensions和space乘积不变
	std::cout<<"origin image dimensions "<<originalDims[0]<< " "<<originalDims[1]<<" "<<originalDims[2]<<std::endl;
	std::cout<<"origin image space: "<<originalSpace[0]<< " "<<originalSpace[1]<<" "<<originalSpace[2]<<std::endl;
	std::cout<<"shrinking image dimensions: "<<shrinkDims[0]<< " "<<shrinkDims[1]<<" "<<shrinkDims[2]<<std::endl;
	std::cout<<"shrinking image space: "<<shrinkSpace[0]<< " "<<shrinkSpace[1]<<" "<<shrinkSpace[2]<<std::endl;
	std::cout<<"up image dimensions: "<<magnifyDims[0]<< " "<<magnifyDims[1]<<" "<<magnifyDims[2]<<std::endl;
	std::cout<<"up image space: "<<magnifySpace[0]<< " "<<magnifySpace[1]<<" "<<magnifySpace[2]<<std::endl;

	return 0;
}
