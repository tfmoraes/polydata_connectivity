# ifndef CONNECTIVIY_H // inicio do guarda
# define CONNECTIVIY_H // inicio do guarda

#include <vtkPLYReader.h>
#include <vtkPolyData.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>

#include <vtkCellPicker.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>

#include <vtkDataSetAttributes.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkLookupTable.h>

#include <list>

#ifdef _WIN32
    extern "C" __declspec(dllexport) void VisitAndMark(vtkPolyData*, vtkIdType, vtkIdType);
#else
    extern "C" void VisitAndMark(vtkPolyData*, vtkIdType, vtkIdType);
#endif

#endif
