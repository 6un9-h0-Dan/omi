#include "py_converter.hpp"


#include "mi_wrapper.hpp"


namespace
{


int
_PyBool_to_MI_ValueBase (
    PyObject* pSource,
    scx::MI_ValueBase::Ptr* ppValueOut)
{
    SCX_BOOKEND ("PyBool_to_MI_ValueBase<TypeID_t>");
    int rval = PY_FAILURE;
    if (NULL != pSource &&
        Py_None != pSource)
    {
        if (PyObject_TypeCheck (
                pSource, scx::MI_Wrapper<MI_BOOLEAN>::getPyTypeObject ()))
        {
            SCX_BOOKEND_PRINT ("PyObject_TypeCheck: true");
            ppValueOut->reset (
                reinterpret_cast<scx::MI_Wrapper<MI_BOOLEAN>*>(
                    pSource)->getValue ());
            rval = PY_SUCCESS;
        }
        else if (PyBool_Check (pSource))
        {
            SCX_BOOKEND_PRINT ("PyBool");
            ppValueOut->reset (
                new scx::MI_Value<MI_BOOLEAN> (
                    Py_False == pSource ? false : true));
            rval = PY_SUCCESS;
        }
    }
    else
    {
        ppValueOut->reset ();
        rval = PY_SUCCESS;
    }
    return rval;
}


template<scx::TypeID_t TYPE>
int
_to_MI_ValueBase (
    PyObject* pSource,
    scx::MI_ValueBase::Ptr* ppValueOut)
{
    SCX_BOOKEND ("to_MI_ValueBase<TypeID_t>");
    int rval = PY_FAILURE;
    if (NULL != pSource &&
        Py_None != pSource)
    {
        if (PyObject_TypeCheck (pSource, scx::MI_Wrapper<TYPE>::getPyTypeObject ()))
        {
            SCX_BOOKEND_PRINT ("PyObject_TypeCheck: true");
            ppValueOut->reset (
                reinterpret_cast<scx::MI_Wrapper<TYPE>*>(pSource)->getValue ());
            rval = PY_SUCCESS;
        }
        else
        {
            SCX_BOOKEND_PRINT ("PyObject_TypeCheck: false");
            typename scx::MI_Type<TYPE>::type_t value;
            rval = scx::fromPyObject (pSource, &value);
            if (PY_SUCCESS == rval)
            {
                ppValueOut->reset (new scx::MI_Value<TYPE> (value));
            }
        }
    }
    else
    {
        ppValueOut->reset ();
        rval = PY_SUCCESS;
    }
    return rval;
}


}


namespace scx
{


// class PyConverter_Sint<long long> specialization definitions
//------------------------------------------------------------------------------
//template<>
/*static*/ PyObject*
PyConverter_Sint<long long>::toPyObject (
    long long const& value)
{
    return PyLong_FromLongLong (value);
}


//template<>
/*static*/ int
PyConverter_Sint<long long>::fromPyObject (
    PyObject* pSource,
    long long* pValueOut)
{
    //SCX_BOOKEND ("PyConverter_Sint<long long>::fromPyObject");
    int rval = PY_FAILURE;
    assert (NULL != pSource && Py_None != pSource);
    assert (NULL != pValueOut);
    if (PyLong_Check (pSource))
    {
        //SCX_BOOKEND_PRINT ("PyLong");
        long long value = PyLong_AsLongLong (pSource);
        if (-1 != value ||
            NULL == PyErr_Occurred ())
        {
            *pValueOut = value;
            rval = PY_SUCCESS;
        }
        else
        {
            // failed
            //SCX_BOOKEND_PRINT ("PyLong_AsLongLong failed");
        }
    }
    else if (PyInt_Check (pSource))
    {
        //SCX_BOOKEND_PRINT ("PyInt");
        long value = PyInt_AsLong (pSource);
        if (-1 != value ||
            NULL == PyErr_Occurred ())
        {
            *pValueOut = static_cast<long long>(value);
            rval = PY_SUCCESS;
        }
        else
        {
            // failed
            //SCX_BOOKEND_PRINT ("PyInt_AsLong failed");
        }
    }
    else
    {
        // failed
        //SCX_BOOKEND_PRINT ("no idea which type");
    }
    return rval;
}


// class PyConverter_Uint<unsigned long long> specialization definitions
//------------------------------------------------------------------------------
/*static*/ PyObject*
PyConverter_Uint<unsigned long long>::toPyObject (
    unsigned long long const& value)
{
    return PyLong_FromUnsignedLongLong (value);
}


/*static*/ int
PyConverter_Uint<unsigned long long>::fromPyObject (
    PyObject* pSource,
    unsigned long long* pValueOut)
{
    //SCX_BOOKEND ("PyConverter_Uint<unsigned long long>::fromPyObject");
    int rval = PY_FAILURE;
    assert (NULL != pSource && Py_None != pSource);
    assert (NULL != pValueOut);
    if (PyLong_Check (pSource))
    {
        //SCX_BOOKEND_PRINT ("PyLong");
        unsigned long long value = PyLong_AsUnsignedLongLong (pSource);
        if ((unsigned long long)-1 != value ||
            NULL == PyErr_Occurred ())
        {
            // succeeded
            *pValueOut = value;
            rval = PY_SUCCESS;
        }
        else
        {
            // failed
            //SCX_BOOKEND_PRINT ("PyLong_AsUnsignedLongLong failed");
        }
    }
    else if (PyInt_Check (pSource))
    {
        //SCX_BOOKEND_PRINT ("PyInt");
        long value = PyInt_AsLong (pSource);
        if (-1 != value ||
            NULL == PyErr_Occurred ())
        {
            if (0 <= value)
            {
                *pValueOut = static_cast<unsigned long long>(value);
                rval = PY_SUCCESS;
            }
            else
            {
                // failed
                //SCX_BOOKEND_PRINT ("value out of range");
            }
        }
        else
        {
            // failed
            //SCX_BOOKEND_PRINT ("PyInt_AsLong failed");
        }
    }
    else
    {
        // failed
        //SCX_BOOKEND_PRINT ("no idea which type");
    }
    return rval;
}


// class PyConverter<MI_Type<MI_STRING>::type_t> specialization definitions
//------------------------------------------------------------------------------
/*static*/ PyObject*
PyConverter<MI_Type<MI_STRING>::type_t>::toPyObject (
    MI_Type<MI_STRING>::type_t const& value)
{
    return PyString_FromString (value.c_str ());
}


/*static*/ int
PyConverter<MI_Type<MI_STRING>::type_t>::fromPyObject (
    PyObject* pSource,
    MI_Type<MI_STRING>::type_t* pValueOut)
{
    //SCX_BOOKEND ("PyConverter<MI_Type<MI_STRING>::type_t>::fromPyObject");
    int rval = PY_FAILURE;
    assert (NULL != pSource && Py_None != pSource);
    assert (NULL != pValueOut);
    if (PyString_Check (pSource))
    {
        *pValueOut = PyString_AsString (pSource);
        rval = PY_SUCCESS;
    }
    else
    {
        // failed
        //SCX_BOOKEND_PRINT ("no idea which type");
    }
    return rval;
}


int
to_MI_ValueBase (
    TypeID_t type,
    PyObject* pSource,
    MI_ValueBase::Ptr* ppValueOut)
{
    SCX_BOOKEND ("to_MI_ValueBase");
    assert (NULL != ppValueOut);
    int rval = PY_FAILURE;
    switch (type)
    {
    case MI_BOOLEAN:
        rval = _PyBool_to_MI_ValueBase (pSource, ppValueOut);
        break;
    case MI_UINT8:
        rval = _to_MI_ValueBase<MI_UINT8> (pSource, ppValueOut);
        break;
    case MI_SINT8:
        rval = _to_MI_ValueBase<MI_SINT8> (pSource, ppValueOut);
        break;
    case MI_UINT16:
        rval = _to_MI_ValueBase<MI_UINT16> (pSource, ppValueOut);
        break;
    case MI_SINT16:
        rval = _to_MI_ValueBase<MI_SINT16> (pSource, ppValueOut);
        break;
    case MI_UINT32:
        rval = _to_MI_ValueBase<MI_UINT32> (pSource, ppValueOut);
        break;
    case MI_SINT32:
        rval = _to_MI_ValueBase<MI_SINT32> (pSource, ppValueOut);
        break;
    case MI_UINT64:
        rval = _to_MI_ValueBase<MI_UINT64> (pSource, ppValueOut);
        break;
    case MI_SINT64:
        rval = _to_MI_ValueBase<MI_SINT64> (pSource, ppValueOut);
        break;
    case MI_REAL32:
        rval = _to_MI_ValueBase<MI_REAL32> (pSource, ppValueOut);
        break;
    case MI_REAL64:
        rval = _to_MI_ValueBase<MI_REAL64> (pSource, ppValueOut);
        break;
    case MI_STRING:
        rval = _to_MI_ValueBase<MI_STRING> (pSource, ppValueOut);
        break;
    default:
        SCX_BOOKEND_PRINT ("encountered an unhandled type");
        break;
    }
    return rval;
}


// // fromPyObject definition
// //------------------------------------------------------------------------------
// int
// fromPyObject (
//     TypeID_t const& type,
//     PyObject* pSource,
//     MI_ValueBase::Ptr* ppValueOut)
// {
//     SCX_BOOKEND ("fromPyObject (TypeID_t, PyObject*, MI_ValueBase::Ptr*)");
//     int rval = PY_FAILURE;
//     assert (NULL != ppValueOut);
// //    switch (type)
// //    {
// //
// //    }
//     return rval;
// }


} // namespace scx
