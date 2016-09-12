# polydata_connectivity
Its a test on access a VTK C++ function using ctypes in Python. It is based on https://github.com/pletzer/vtkpythoncpp. The idea is to use GetAddressAsString from any VTK object inside python, convert to long (using ctypes.c_long(addr)) and pass it to the C++ function as if it were a VTK C++ Object pointer.
