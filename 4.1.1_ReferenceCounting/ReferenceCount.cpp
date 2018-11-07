//���ü����Ǹ��򵥵�����������ϵ��ֻҪ������������ĳ����(��Ϊ����o)������o�ͻ����һ�����ü�����
//��������ö���o�Ķ����Ƴ���o����ͻ��Զ�������
//vtk��ʹ�����ü����ĺô��ǣ�����ʵ������֮��Ĺ�������ÿ������Ӷ��ﵽ��ʡ�ڴ��Ŀ��

#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageData.h>

int main(int argc, char*argv[])
{

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();  // ���ȴ���һ��vtkReader����
	// vtkSmartPointer<vtkJPEGReader> reader = vtkJPEGReader::New(); ����˵���ǲ��ܽ�ԭʼָ�봫�ݸ�����ָ�룬�������ܻ�����ڴ�й©������
	reader->SetFileName("hua.jpg");  // Ȼ���ȡһ��jpgͼ��
	reader->Update();  // this algorithm's outputs up-to-date.

	std::cout<<"Reference Count of reader->GetOutput (BeforeAssignment) = "
		<<reader->GetOutput()->GetReferenceCount()<<std::endl;  // Return the current reference count of this object.
	vtkSmartPointer<vtkImageData> image1 = reader->GetOutput();
	
	std::cout<<"Reference Count of reader->GetOutput (Assignto image1) = "
		<<reader->GetOutput()->GetReferenceCount()<<std::endl;
	std::cout<<"Reference Count of image1 = "
		<<image1->GetReferenceCount()<<std::endl;
	
	vtkSmartPointer<vtkImageData> image2 = reader->GetOutput();
	std::cout<<"Reference Count of reader->GetOutput (Assignto image2) = "
		<<reader->GetOutput()->GetReferenceCount()<<std::endl;
	std::cout<<"Reference Count of image2 = "
		<<image2->GetReferenceCount()<<std::endl;
	
	return 0;

}
