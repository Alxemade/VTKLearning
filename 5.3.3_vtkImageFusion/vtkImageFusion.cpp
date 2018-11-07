#include <vtkSmartPointer.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkJPEGReader.h>
#include <vtkImageBlend.h>
#include <vtkImageData.h>

int main(int argn, char *argv[])
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();  // ���ȶ�ȡһ��ͼ�����
	reader->SetFileName ("hua.jpg" );  // Ȼ��ʼ��ȡһ��ͼ��
	reader->Update();  // ��ʼ���ӻ����ߣ�Ȼ����и���

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();  //  vtkImageCanvasSource2D is a source that starts as a blank image
	imageSource->SetNumberOfScalarComponents(1); // ���ù�һ�������Ϊ1
	imageSource->SetScalarTypeToUnsignedChar();  // ���ù�һ���ĳ߶ȵ�����Ϊunsigned char����
	imageSource->SetExtent(0, 512, 0, 512, 0, 0);  // set the WholeExtent of the output���������ȫ������
	imageSource->SetDrawColor(0.0);  // Set DrawColor to (a, 0, 0, 0)
	imageSource->FillBox(0, 512, 0, 512);  //  Set the pixels inside the box (min0, max0, min1, max1) to the current
	imageSource->SetDrawColor(255.0);  // Set  DrawColor  to (a, 0��0��0��
	imageSource->FillBox(100,400,100,400);  // �ڻ���һ�������κ���
	imageSource->Update();  // ���¿��ӻ�����

	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();  // blend images together using alpha or opacity
	imageBlend->AddInputData(0, reader->GetOutput());
	imageBlend->AddInputData(1, imageSource->GetOutput());
	imageBlend->SetOpacity(0, 0.4); // ���˿�0���ò�͸����0.4
	imageBlend->SetOpacity(1, 0.6); //  ���˿�1���ò�͸��Ϊ0.6
	imageBlend->Update();  // ����vtkImageBlend
	return 0;
}

