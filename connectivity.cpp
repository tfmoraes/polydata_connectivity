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

#include "connectivity.h"

extern "C"
void VisitAndMark(vtkPolyData* mesh, vtkIdType pid, vtkIdType value){
    vtkPointData *pd = mesh->GetPointData();
    vtkIdTypeArray *scalars;
    vtkIdType numPts, npts;
    vtkIdType idcell, idpoint, *ptIds;
    vtkIdList *cellIds;
    std::list<vtkIdType> to_visit;
    int i, j;
    if (pd->GetNumberOfArrays()){
        std::cout << "Tem array" << std::endl;
        scalars = static_cast<vtkIdTypeArray*>(pd->GetArray("RegionID"));
        pd->SetActiveAttribute("RegionID", vtkDataSetAttributes::SCALARS);
    }
    else{
        numPts = mesh->GetNumberOfPoints();
        scalars = vtkIdTypeArray::New();
        scalars->SetName("RegionID");
        scalars->SetNumberOfValues(numPts);
        int idx = pd->AddArray(scalars);
        pd->SetActiveAttribute(idx, vtkDataSetAttributes::SCALARS);

        for(i=0; i < mesh->GetNumberOfPoints(); i++){
            scalars->SetValue(i, 0);
        }
    }


    mesh->GetCellPoints(pid, npts, ptIds);

    for(j=0; j < npts; j++){
        idpoint = ptIds[ j ];
        scalars->SetValue(idpoint, value);
        to_visit.push_back(idpoint);
    }

    cellIds = vtkIdList::New();
    while(!to_visit.empty()){
        pid = to_visit.front();
        to_visit.pop_front();
        mesh->GetPointCells(pid, cellIds);
        for(i=0; i < cellIds->GetNumberOfIds(); i++){
            idcell = cellIds->GetId(i);
            mesh->GetCellPoints(idcell, npts, ptIds);

            for(j=0; j < npts; j++){
                idpoint = ptIds[ j ];

                if (scalars->GetValue(idpoint) != value){
                    scalars->SetValue(idpoint, value);
                    to_visit.push_back(idpoint);
                }
            }

        }
    }

    // mesh->Update();
}

