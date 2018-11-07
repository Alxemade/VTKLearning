#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();  // 获取jpeg图像
	reader->SetFileName ( "hua.jpg" );
	reader->Update();  // 立即更新vtk管线

	int dims[3];  // 定义存储的三维坐标
	reader->GetOutput()->GetDimensions(dims); // 这里是获取了该图像的维度了，然后将这个赋值给dims这个数组

	for(int k=0; k<dims[2]; k++)  // 循环第三维
	{
		for(int j=0; j<dims[1]; j++)
		{
			for(int i=0; i<dims[0]; i++)  // 循环一幅图像通过i和j
			{
				if(i<100 && j<100)  // 限定区域是100*100的范围之内，默认图像的原点是处在左下角
				{
					unsigned char * pixel =
						(unsigned char *) (reader->GetOutput()->GetScalarPointer(i, j, k) );  // GetScalarPointer获取的指针是void *类型，这里需要强制类型转换
					*pixel     = 0;  // 这里是RGB图像，所以是通过指针进行访问的  R分量=0
					*(pixel+1) = 0;  //  G分量=0
				 	*(pixel+2) = 0;  //  B分量=0 , 所以最后产生的是一个黑色图像
				}
			}
		}
	}

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor=
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	renderWindowInteractor->Start();

}

