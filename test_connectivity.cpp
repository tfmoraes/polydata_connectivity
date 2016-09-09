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

#include <vtkCellPicker.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>

#include <vtkDataSetAttributes.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkLookupTable.h>

#include <list>

#include "connectivity.h"


void LeftClickCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
  vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
  vtkPolyData *mesh = static_cast<vtkPolyData*>(clientData);
  vtkCellPicker *picker = static_cast<vtkCellPicker*>(iren->GetPicker());

  std::cout << "Event ID " << eventId << std::endl;
  if (iren->GetControlKey()){
      int x, y;
      iren->GetEventPosition(x, y);

      picker->Pick(x, y, 0, iren->FindPokedRenderer(x, y));
      int pid = picker->GetCellId();
      std::cout << pid << std::endl;
      VisitAndMark(mesh, pid, 1);
      iren->Render();
  }
  else if (iren->GetShiftKey()){
      int x, y;
      iren->GetEventPosition(x, y);

      picker->Pick(x, y, 0, iren->FindPokedRenderer(x, y));
      int pid = picker->GetCellId();
      std::cout << pid << std::endl;
      VisitAndMark(mesh, pid, 0);
      iren->Render();
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
  
  vtkLookupTable *lut = vtkLookupTable::New();
  lut->SetNumberOfTableValues(100);
  lut->Build();
  lut->SetTableValue(0, 1.0, 1.0, 1.0, 1.0);
  lut->SetTableValue(99, 1.0, 0.0, 0.0, 1.0);

  // Create a mapper.
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
  mapper->SetInputData(reader->GetOutput());
  //mapper->ScalarVisibilityOn();
  mapper->SetScalarRange(0, 1);
  mapper->SetLookupTable(lut);
  
  // Create the actor.
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);

  // Picker
  vtkCellPicker* picker = vtkCellPicker::New();
  
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

  vtkXMLPolyDataWriter *w = vtkXMLPolyDataWriter::New();
  w->SetInputData(reader->GetOutput());
  w->SetFileName("/tmp/saida.vtp");
  w->Write();
  
  actor->Delete();
  mapper->Delete();
  reader->Delete();
  renWin->Delete();
  ren->Delete();
  iren->Delete();

  return 0;
}
