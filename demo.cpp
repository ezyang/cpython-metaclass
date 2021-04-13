#include <Python.h>
#include <iostream>

struct foometa {
  PyTypeObject head;
};

int foometa_init(foometa *cls, PyObject *args, PyObject *kwargs) {
  if (PyType_Type.tp_init((PyObject*)cls, args, kwargs) < 0) {
    return -1;
  }
  std::cerr << "I'm called on class construction time!\n";
  return 0;
}

#define DEFERRED_ADDRESS(ADDR) nullptr

static PyTypeObject foometa_type = {
    PyVarObject_HEAD_INIT(DEFERRED_ADDRESS(&PyType_Type), 0)
    "demo.foometa",
    0,
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                           /* tp_methods */
    0,                                          /* tp_members */
    0,                           /* tp_getset */
    DEFERRED_ADDRESS(&PyType_Type),             /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)foometa_init,                    /* tp_init */
    0,                                          /* tp_alloc */
    0                                           /* tp_new */
};

PyObject *fooparent_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  PyObject* obj = type->tp_alloc(type, 0);
  return obj;
}

static PyTypeObject fooparent_type = {
    PyVarObject_HEAD_INIT(&foometa_type, 0)
    "demo.fooparent",
    sizeof(PyObject),
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                           /* tp_methods */
    0,                                          /* tp_members */
    0,                           /* tp_getset */
    0,             /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                    /* tp_init */
    0,                                          /* tp_alloc */
    fooparent_new                                           /* tp_new */
};

int
demo_init(PyObject *m) {
  foometa_type.tp_base = &PyType_Type;
  if (PyType_Ready(&foometa_type) < 0) {
    return -1;
  }
  if (PyType_Ready(&fooparent_type) < 0) {
    return -1;
  }

  Py_INCREF(&foometa_type);
  if (PyModule_AddObject(m, "foometa",
                         (PyObject *) &foometa_type) < 0)
      return -1;

  Py_INCREF(&fooparent_type);
  if (PyModule_AddObject(m, "fooparent",
                         (PyObject *) &fooparent_type) < 0)
      return -1;
  return 0;
}

static PyModuleDef demomodule = {
    PyModuleDef_HEAD_INIT,
    "demo",
    "Example module",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_demo() {
  PyObject* m = PyModule_Create(&demomodule);
  if (m == nullptr) return nullptr;
  if (demo_init(m) < 0) return nullptr;
  return m;
}
