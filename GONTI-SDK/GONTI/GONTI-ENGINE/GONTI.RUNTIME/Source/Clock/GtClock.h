#ifndef GtClockH
#define GtClockH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "GtClockTypes.inl"

        GTAPI void gontiVkClockUpdate(Clock* clock);
        GTAPI void gontiVkClockStart(Clock* clock);
        GTAPI void gontiVkClockStop(Clock* clock);

#ifdef __cplusplus
    }
#endif

#endif