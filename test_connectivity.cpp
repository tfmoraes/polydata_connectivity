/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: TestPLYReader.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME Test of vtkPLYReader
// .SECTION Description
//

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

#include <vtkPointPicker.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>

#include <vtkDataSetAttributes.h>

#include <vtkInteractorStyleTrackballCamera.h>


void VisitAndMark(vtkPolyData* mesh, vtkIdType pid, vtkIdType value){
    vtkPointData *pd = mesh->GetPointData();
    vtkIdTypeArray *scalars;
    vtkIdType numPts, npts;
    vtkIdType idcell, idpoint, *ptIds;
    vtkIdList *cellIds ;
    int i, j;
    if (pd->GetNumberOfArrays()){
        std::cout << "Tem array" << std::endl;
        scalars = static_cast<vtkIdTypeArray*>(pd->GetArray("RegionID"));
    }
    else{
        numPts = mesh->GetNumberOfPoints();
        scalars = vtkIdTypeArray::New();
        scalars->SetName("RegionID");
        scalars->SetNumberOfValues(numPts);
        int idx = pd->AddArray(scalars);
        pd->SetActiveAttribute(idx, vtkDataSetAttributes::SCALARS);
        std::cout << "Não tem array" << std::endl;
    }

    mesh->GetPointCells(pid, cellIds);
    for(i=0; i < cellIds->GetNumberOfIds(); i++){
        idcell = cellIds->GetId(i);
        std::cout << "idcell " << idcell << std::endl; 
        mesh->GetCellPoints(idcell, npts, ptIds);

        for(j=0; j < npts; j++){
            idpoint = ptIds[ j ];
            std::cout << "ID " << idpoint << std::endl;
            scalars->SetValue(idpoint, value);
        }

    }
}

void LeftClickCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
  vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
  vtkPolyData *mesh = static_cast<vtkPolyData*>(clientData);
  vtkPointPicker *picker = static_cast<vtkPointPicker*>(iren->GetPicker());

  std::cout << "Event ID " << eventId << std::endl;
  if (iren->GetControlKey()){
      int x, y;
      iren->GetEventPosition(x, y);

      picker->Pick(x, y, 0, iren->FindPokedRenderer(x, y));
      int pid = picker->GetPointId();
      std::cout << pid << std::endl;
      VisitAndMark(mesh, pid, 1);

  }
}

int main( int argc, char *argv[] )
{
   printf("\nargc %d\n", argc);
  // Read file name.
  char* fname = argv[1];

  // Create the reader.
  vtkPLYReader* reader = vtkPLYReader::New();
  reader->SetFileName(fname);
  reader->Update();
  reader->GetOutput()->BuildLinks();
  
  // Create a mapper.
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
  mapper->SetInput(reader->GetOutput());
  mapper->ScalarVisibilityOn();
  
  // Create the actor.
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);

  // Picker
  vtkPointPicker* picker = vtkPointPicker::New();
  
  // Basic visualisation.
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  vtkRenderer* ren = vtkRenderer::New();
  renWin->AddRenderer(ren);
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);
  iren->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
  iren->SetPicker(picker);
  
  ren->AddActor(actor);
  ren->SetBackground(0,0,0);
  renWin->SetSize(300,300);

  vtkSmartPointer<vtkCallbackCommand> lefclickCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  lefclickCallback->SetCallback(LeftClickCallbackFunction );
  lefclickCallback->SetClientData(reader->GetOutput());
  iren->AddObserver(vtkCommand::LeftButtonPressEvent, lefclickCallback);

  // interact with data
  renWin->Render();
  iren->Start();
  
  actor->Delete();
  mapper->Delete();
  reader->Delete();
  renWin->Delete();
  ren->Delete();
  iren->Delete();

  return 0;
}
