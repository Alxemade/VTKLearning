//引用计数是个简单的垃圾回收体系，只要其它对象引用某对象(记为对象o)，对象o就会存在一个引用计数，
//当最后引用对象o的对象移除，o对象就会自动析构。
//vtk里使用引用计数的好处是，可以实现数据之间的共享而不用拷贝，从而达到节省内存的目的

#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageData.h>

int main(int argc, char*argv[])
{

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();  // 首先创建一个vtkReader的类
	// vtkSmartPointer<vtkJPEGReader> reader = vtkJPEGReader::New(); 就是说我们不能将原始指针传递给智能指针，这样可能会出现内存泄漏的问题
	reader->SetFileName("hua.jpg");  // 然后读取一个jpg图像
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
