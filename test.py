import ctypes
import sys

import vtk

lib = ctypes.cdll.LoadLibrary('./libconnectivity.so')


class TestConnectivity:
    def __init__(self, ply_file):
        reader = vtk.vtkPLYReader()
        reader.SetFileName(ply_file)
        reader.Update()
        reader.GetOutput().BuildLinks()

        self.mesh = reader.GetOutput()

        lut = vtk.vtkLookupTable()
        lut.SetNumberOfTableValues(100)
        lut.Build()
        lut.SetTableValue(0, 1.0, 1.0, 1.0, 1.0)
        lut.SetTableValue(99, 1.0, 0.0, 0.0, 1.0)

        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(reader.GetOutput())
        mapper.SetScalarRange(0, 1)
        mapper.SetLookupTable(lut)

        actor = vtk.vtkActor()
        actor.SetMapper(mapper)

        picker = vtk.vtkCellPicker()

        ren = vtk.vtkRenderer()
        ren.AddActor(actor)

        renWin = vtk.vtkRenderWindow()
        renWin.AddRenderer(ren)
        renWin.SetSize(300, 300)

        iren = vtk.vtkRenderWindowInteractor()
        iren.SetRenderWindow(renWin)
        iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
        iren.SetPicker(picker)

        iren.AddObserver('LeftButtonPressEvent', self.LeftClick)

        iren.Start()

        writer = vtk.vtkXMLPolyDataWriter()
        writer.SetInputData(self.mesh)
        writer.SetFileName('/tmp/manolo.vtp')
        writer.Write()

    def LeftClick(self, obj, evt):
        picker = obj.GetPicker()
        x, y = obj.GetEventPosition()

        if obj.GetControlKey() or obj.GetShiftKey():
            picker.Pick(x, y, 0, obj.FindPokedRenderer(x, y))
            pid = picker.GetCellId()

            if pid == -1:
                return

            if obj.GetControlKey():
                val = 1
            else:
                val = 0

            addr = int(self.mesh.GetAddressAsString('vtkPolyData')[5:], 16)
            lib.VisitAndMark(ctypes.c_long(addr), pid, val)

            print pid, addr



def main():
    TestConnectivity(sys.argv[1])


if __name__ == "__main__":
    main()
