/*
 * FILE: osapi_time.h - Definition of Time API
 *
 */
/*e \file
 * \brief Time API definition
 *
 * \details
 * This file implements function to convert between the NtpTime format and
 * other formats.
 */
#ifndef osapi_time_h
#define osapi_time_h

#include "osapi/osapi_dll.h"
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \defgroup OSAPITimeClass OSAPI Time
 *  \ingroup OSAPIModule
 */

/*e \ingroup OSAPITimeClass
 *
 *   \brief NtpTime API.
 */
/* ================================================================= */
/*                           Definition: Time                        */
/* ----------------------------------------------------------------- */

/*e \ingroup OSAPITimeClass
 *
 * @brief NTP Time representation.
 *
 * Expresses time in NTP format. The second field is simply an integer
 * expressing seconds. The fraction field expresses 1/2^32 of a second.
 * We strongly urge customers to use our provided macros to convert this
 * format to and from human readable form.
 *
 * \b Example:
 *
 * The following is a simple example on how to prepare a struct OSAPI_NtpTime
 * structure to be 1.5 seconds.
 *
 * \verbatim
 *   struct OSAPI_NtpTime ntpTime;
 *
 *   OSAPI_NtpTime_from_millisec(ntpTime, 1, 500);
 * \endverbatim
 */
typedef struct OSAPI_NtpTime
{
    /* e Seconds.* */
    OEI_INT32 sec;
    /* e fraction of a second in 1/2^32 form. */
    OEI_UINT32 frac;
} OSAPI_NtpTime;

/*e \ingroup OSAPITimeClass
 * The maximum number of seconds that can be represented using NTP time.
 */
#define OSAPI_NTP_TIME_SEC_MAX ((OEI_INT32)0x7fffffff)

/*e \ingroup OSAPITimeClass
 * The largest possible value of the fraction field in NTP time.
 */
#define OSAPI_NTP_TIME_FRAC_MAX ((OEI_UINT32)0xffffffff)


/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from seconds and milliseconds to struct OSAPI_NtpTime format.
 *
 * \details
 * This macro assumes that msec < 1000. It is the caller's responsibility
 * to ensure this.
 *
 * @param time  Contains the answer.
 * @param s     Seconds to convert.
 * @param msec  Milliseconds to convert.
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_to_millisec
 */
OSAPIDllExport void
OSAPI_NtpTime_from_millisec(struct OSAPI_NtpTime *const time, OEI_INT32 s, OEI_UINT32 msec);

/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from struct OSAPI_NtpTime to seconds and milliseconds.
 *
 * \details
 * This macro does not check for overflow, so for a near-infinite time value,
 * the conversion result may end up being negative. It is the responsibility
 * of the user to avoid passing these large time values.
 *
 * @param s     Holds the seconds answer.
 * @param msec  Holds the millisecond answer.
 * @param time  The time to convert to second(s) and millisecond(s).
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_from_millisec
 */
OSAPIDllExport void
OSAPI_NtpTime_to_millisec(OEI_INT32 *const s, OEI_UINT32 *const msec,
                          const struct OSAPI_NtpTime *const time);

/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from seconds and microseconds to struct OSAPI_NtpTime format.
 *
 * @details
 * This macro does not check for overflow, so for a near-infinite time value,
 * the conversion result may end up being negative. It is the caller's
 * responsibility to avoid passing these large time values.
 *
 * @param time  Contains the answer.
 * @param s     Seconds to covert.
 * @param usec  Microseconds to convert.
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_to_millisec
 */
OSAPIDllExport void
OSAPI_NtpTime_from_microsec(struct OSAPI_NtpTime *const time, OEI_INT32 s, OEI_UINT32 usec);

/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from struct OSAPI_NtpTime to seconds and microseconds.
 *
 * @param s    Holds the second portion.
 * @param usec Holds the microsecond fraction.
 * @param time The time to convert to second(s) and microsecond(s)
 *
 * This macro does not check for overflow, so for a near-infinite time value,
 * the conversion result may end up being negative. It is the caller's
 * responsibility to avoid passing these large time values.
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_from_millisec
 */
OSAPIDllExport void
OSAPI_NtpTime_to_microsec(OEI_INT32 *const s, OEI_UINT32 *const usec,
                          const struct OSAPI_NtpTime *const time);

/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from seconds and nanoseconds to struct OSAPI_NtpTime format.
 *
 * @details
 * This macro assumes that nsec < 1000000000. It is the caller's responsibility
 * to ensure this. The accuracy of this conversion over the whole range of
 * nanosecond values is at least 1 nanosecond.
 *
 * @param time Holds the answer.
 * @param s    Seconds to convert.
 * @param nsec Nanoseconds to convert.
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_to_nanosec
 */
OSAPIDllExport void
OSAPI_NtpTime_from_nanosec(struct OSAPI_NtpTime *const time, OEI_INT32 s, OEI_UINT32 nsec);


/*e \ingroup OSAPITimeClass
 *
 * @brief Macro to convert from struct OSAPI_NtpTime to seconds and nanoseconds.
 *
 * @details
 * The accuracy of this conversion over the whole range of
 * struct OSAPI_NtpTime values is at least 1 nanosecond.
 *
 * @param s     Holds the second portion.
 * @param nsec  Holds the nanosecond portion.
 * @param time  Time to convert to second(s) and nanosecond(s).
 *
 * @see struct OSAPI_NtpTime OSAPI_NtpTime_from_nanosec
 */
OSAPIDllExport void
OSAPI_NtpTime_to_nanosec(OEI_INT32 *const s, OEI_UINT32 *const nsec,
                         const struct OSAPI_NtpTime *const time);

/*e \ingroup OSAPITimeClass
 *
 * A NULL struct OSAPI_NtpTime pointer is considered infinity. This is consistent
 * with the concept of infinite time on UNIX systems.
 *
 * In addition, if the seconds field equals OSAPI_NTP_TIME_SEC_MAX, the
 * time value is also considered infinite.
 *
 * @param time Pointer to OEITime.
 *
 * @return OEI_TRUE if time is infinite, OEI_FALSE otherwise.
*/
OSAPIDllExport OEI_BOOL
OSAPI_NtpTime_is_infinite(const struct OSAPI_NtpTime *const time);

/*e \ingroup OSAPITimeClass
 *
 * @brief answer = t1 - t2.
 *
 * @param answer struct OSAPI_NtpTime
 * @param t1 struct OSAPI_NtpTime`
 * @param t2 struct OSAPI_NtpTime
 */
OSAPIDllExport void
OSAPI_NtpTime_subtract(struct OSAPI_NtpTime *const answer,
                       const struct OSAPI_NtpTime *const t1,
                       const struct OSAPI_NtpTime *const t2);

/*e \ingroup OSAPITimeClass
 *
 * @brief answer += time.
 *
 * This macro does not check for overflow.
 *
 * @param answer struct OSAPI_NtpTime
 * @param time struct OSAPI_NtpTime
 */
#define OSAPI_NtpTime_increment(answer, time) \
{ \
    register OEI_UINT32 currentFrac = (answer)->frac; \
    (answer)->sec  += (time)->sec; \
    (answer)->frac += (time)->frac; \
    if (((answer)->frac < (time)->frac) || ((answer)->frac < currentFrac)) { \
        (answer)->sec++; \
    } \
}

/*e \ingroup OSAPITimeClass
 *
 * @brief Decrement one struct OSAPI_NtpTime value by another struct OSAPI_NtpTime value.
 *
 * Precondition:
 * Postcondition: answer -= time.
 *
 * @param answer struct OSAPI_NtpTime
 * @param time struct OSAPI_NtpTime
 *
 */
#define OSAPI_NtpTime_decrement(answer, time) \
{ \
    register OEI_UINT32 currentFrac = (answer)->frac; \
    (answer)->sec  -= (time)->sec; \
    (answer)->frac -= (time)->frac; \
    if (((answer)->frac > currentFrac)) { (answer)->sec--; } \
}

/*e \ingroup OSAPITimeClass
 *
 * @brief Zero time.
 *
 * This global variable is for convenience. It allows you to see if a
 * OEITime variable is negative or positive by comparing against this.
 *
 * @see OEITimeCompare
 */
#define OSAPI_NTP_TIME_ZERO {0,0}

/*e \ingroup OSAPITimeClass
 *
 * Represents the maximum timevalue that can be represented using the
 * NTP time format. For all practical purposes, it can be considered
 * equivalent to infinity.
 */
#define OSAPI_NTP_TIME_MAX {OSAPI_NTP_TIME_SEC_MAX,OSAPI_NTP_TIME_FRAC_MAX}

/*e \ingroup OSAPITimeClass
 *  The number of nanoseconds per second. 1e9.
 *  @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_NSEC_PER_SEC  (1000000000)

/*e \ingroup OSAPITimeClass
 *  The number of microseconds per second. 1e6.
 * @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_USEC_PER_SEC  (1000000)

/*e \ingroup OSAPITimeClass
 *  The number of milliseconds per second. 1e3.
 *  @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_MSEC_PER_SEC  (1000)

/*e \ingroup OSAPITimeClass
 * The number of microseconds per milliseconds. 1e3.
 * @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_NSEC_PER_USEC (1000)

/*e \ingroup OSAPITimeClass
 * The number of microseconds per milliseconds. 1e3.
 * @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_USEC_PER_MSEC (1000)

/*e \ingroup OSAPITimeClass
 * The number of seconds per second. 1.
 * @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_SEC_PER_SEC   (1)

/*e \ingroup OSAPITimeClass
 * The number of nano seconds per milli second. 1e6.
 * @see OEITimePack OEITimeUnpack
 */
#define OSAPI_NTP_TIME_NSEC_PER_MSEC   (1000000)

#ifdef __cplusplus
}                  /* extern "C" */
#endif

#endif /* osapi_time_h */
