#ifdef USE_DEDICATED_GPU
extern "C" {
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif