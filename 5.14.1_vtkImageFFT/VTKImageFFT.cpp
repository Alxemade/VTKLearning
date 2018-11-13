#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageFFT.h>
#include <vtkImageRFFT.h>
#include <vtkImageCast.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main()
{
	vtkSmartPointer<vtkJPEGReader>reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ( "hua2.jpg" );  // ���ȶ�ȡһ��ͼ��
	reader->Update();  

	vtkSmartPointer<vtkImageFFT> fftFilter =
		vtkSmartPointer<vtkImageFFT>::New();  // Fast Fourier Transform.
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->Update();   // �������ͼ����п���fft�任���õ���ͼ���Ǹ���ͼ���ǲ���ֱ����ʾ�ģ�����������Ҫ��ȡĳһ���ֵ�ͼ��
	// �������ȡ��֣�Ȼ�����������ֻ��Ϊ����ʾ����
	vtkSmartPointer<vtkImageExtractComponents> fftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();  // Outputs a single component
	fftExtractReal->SetInputConnection(fftFilter->GetOutputPort());
	fftExtractReal->SetComponents(0);  // ����������ȡ��ʵ��ͼ��
	// ��ΪvtkImageActorֻ����ʾunsigned char���͵�ͼ������������Ҫ�����������ݱ��unsigned char����
	vtkSmartPointer<vtkImageCast> fftCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	fftCastFilter->SetInputConnection(fftExtractReal->GetOutputPort());
	fftCastFilter->SetOutputScalarTypeToUnsignedChar();  // ������unsigned char��������
	fftCastFilter->Update();
	// ���ֱ����Ҫ���и���Ҷ���任�Ļ�����ô��ֱ������Ƶ����ռ������
	vtkSmartPointer<vtkImageRFFT> rfftFilter =
		vtkSmartPointer<vtkImageRFFT>::New();  //  Reverse Fast Fourier Transform.
	rfftFilter->SetInputConnection(fftFilter->GetOutputPort()); 
	rfftFilter->Update();  // ��

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);  // ��ȡ��һ�����

	vtkSmartPointer<vtkImageCast> rfftCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	rfftCastFilter->SetInputConnection(ifftExtractReal->GetOutputPort());
	rfftCastFilter->SetOutputScalarTypeToUnsignedChar();  // ����ȡ�����ݱ��Unsigned char����
	rfftCastFilter->Update();

	// ���������ʾ��һЩ����
	vtkSmartPointer<vtkImageActor> inputActor =
		vtkSmartPointer<vtkImageActor>::New();
	inputActor->SetInputData(reader->GetOutput());  // ��ʾ����ԭͼ��

	vtkSmartPointer<vtkImageActor> centerActor =
		vtkSmartPointer<vtkImageActor>::New();
	centerActor->SetInputData(fftCastFilter->GetOutput());  // ��ʾ��ͼ���Ƶ������Ϣ

	vtkSmartPointer<vtkImageActor> extractedActor =
		vtkSmartPointer<vtkImageActor>::New();
	extractedActor->SetInputData(rfftCastFilter->GetOutput());

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 300);  // ������Ⱦ���ڵĴ�С��600*300

	// And one interactor�� ����ʹ��һ����Ⱦ���ڽ�����
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	double leftViewport[4] = { 0.0, 0.0, 0.33, 1.0 };
	double centerViewPort[4] = { 0.33, 0.0, 0.66, 1.0 };
	double rightViewport[4] = { 0.66, 0.0, 1.0, 1.0 };

	// Setup both renderers����Ȼʹ����һ����Ⱦ���ڣ�����������Ҫ���ö�����Ⱦ��
	vtkSmartPointer<vtkRenderer> leftRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // ������ߵ���Ⱦ��
	renderWindow->AddRenderer(leftRenderer);  // �������ǽ�������ߵ���Ⱦ����ӵ���Ⱦ��������ȥ
	leftRenderer->SetViewport(leftViewport);  // �������ǵ������Ⱦ�����Ӵ�
	leftRenderer->SetBackground(.6, .5, .4);  // ������ߵ���Ⱦ���ı�����ɫ

	vtkSmartPointer<vtkRenderer> centerRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // �����м����Ⱦ��
	renderWindow->AddRenderer(centerRenderer);  // �������ǽ������м����Ⱦ����ӵ���Ⱦ��������ȥ
	centerRenderer->SetViewport(centerViewPort);  // �������ǵ��м���Ⱦ�����Ӵ�
	centerRenderer->SetBackground(.4, .6, .5);  // �����м����Ⱦ���ı�����ɫ

	vtkSmartPointer<vtkRenderer> rightRenderer =
		vtkSmartPointer<vtkRenderer>::New();  // �����ұߵ���Ⱦ��
	renderWindow->AddRenderer(rightRenderer); // ���ұ���Ⱦ����ӵ���Ⱦ��������
	rightRenderer->SetViewport(rightViewport); // ������Ⱦ�����Ӵ�
	rightRenderer->SetBackground(.4, .5, .6);  // ������Ⱦ���ı���

	leftRenderer->AddActor(inputActor);  // ��������Ⱦ�����Actor
	centerRenderer->AddActor(centerActor); // ���м����Ⱦ�����Actor
	rightRenderer->AddActor(extractedActor);  // ���Ҳ����Ⱦ�����Actor

	leftRenderer->ResetCamera();  // Automatically set up the camera based on the visible actors.
	centerRenderer->ResetCamera();
	rightRenderer->ResetCamera(); //Automatically set up the camera based on the visible actors.
	//Ҳ���ǿ�ʼ����vtk����
	renderWindow->Render();  //  Ask each renderer owned by this RenderWindow to render its image and synchronize this process.
	interactor->Start();  // Start the event loop

	return 0;
}

