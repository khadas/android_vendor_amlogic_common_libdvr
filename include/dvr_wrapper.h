/**
 * \mainpage Amlogic DVR library
 *
 * \section Introduction
 * "libdvr" is a library provides basic DVR functions used by Amlogic platform.
 * It supports:
 * \li Record
 * \li Playback
 * \li Index file generated
 * \li Segment split
 * \li Encrypt and decrypt
 * \endsection
 *
 * \file
 * \brief libdvr wrapper layer
 *
 * Wrapper layer is upper layer of libdvr.
 * It is on top of dvr_record and dvr_playback.
 * It supports:
 * \li Separate record segments automatically.
 * \li Load segments automatically.
 */

#ifndef DVR_WRAPPER_H_
#define DVR_WRAPPER_H_

#include "dvr_types.h"
#include "dvr_crypto.h"
#include "dvr_playback.h"
#include "dvr_record.h"

#ifdef __cplusplus
extern "C" {
#endif

/**Record wrapper handle.*/
typedef void* DVR_WrapperRecord_t;
/**Playback wrapper handle.*/
typedef void* DVR_WrapperPlayback_t;

typedef void* Playback_DeviceHandle_t;

typedef struct {
  time_t              time;       /**< time duration, unit on ms*/
  loff_t              size;       /**< size*/
  uint32_t            pkts;       /**< number of ts packets*/
} DVR_WrapperInfo_t;

typedef struct {
  uint32_t              nb_pids;                         /**< Number of PIDs.*/
  DVR_StreamPid_t       pids[DVR_MAX_RECORD_PIDS_COUNT]; /**< PIDs to be recorded.*/
} DVR_WrapperPidsInfo_t;

typedef struct {
  DVR_PlaybackPlayState_t state;
  DVR_WrapperInfo_t info_cur;
  DVR_WrapperInfo_t info_full;
  DVR_PlaybackPids_t pids;
  float speed;
  DVR_PlaybackSegmentFlag_t flags;
} DVR_WrapperPlaybackStatus_t;

typedef struct {
  DVR_RecordState_t state;            /**< DVR record state*/
  DVR_WrapperInfo_t info;             /**< DVR record information*/
  DVR_WrapperPidsInfo_t pids;         /**< DVR record pids info*/
} DVR_WrapperRecordStatus_t;

/**Record wrapper open parameters.*/
typedef struct {
  int                   dmx_dev_id;                      /**< Demux device's index.*/
  char                  location[DVR_MAX_LOCATION_SIZE]; /**< Location of the record file.*/
  DVR_Bool_t            is_timeshift;                    /**< The record file is used by timeshift.*/
  loff_t                segment_size;                    /**< Segment file's size.*/
  loff_t                max_size;                        /**< Maximum record file size in bytes.*/
  time_t                max_time;                        /**< Maximum record time in seconds.*/
  DVR_RecordFlag_t      flags;                           /**< Flags.*/
  DVR_CryptoPeriod_t    crypto_period;                   /**< Crypto period.*/
  DVR_CryptoFunction_t  crypto_fn;                       /**< Crypto callback function.*/
  void                 *crypto_data;                     /**< User data of crypto function.*/
  DVR_RecordEventFunction_t   event_fn;                  /**< DVR record event callback function*/
  void                        *event_userdata;           /**< DVR event userdata*/
} DVR_WrapperRecordOpenParams_t;

typedef struct {
  DVR_WrapperPidsInfo_t pids_info;
} DVR_WrapperRecordStartParams_t;

typedef struct {
  uint32_t              nb_pids;                               /**< Number of PID actions.*/
  DVR_StreamPid_t       pids[DVR_MAX_RECORD_PIDS_COUNT];       /**< PIDs.*/
  DVR_RecordPidAction_t pid_action[DVR_MAX_RECORD_PIDS_COUNT]; /**< Actions.*/
} DVR_WrapperUpdatePidsParams_t;

/**Playback wrapper open parameters.*/
typedef struct {
  int                     dmx_dev_id;                      /**< playback used dmx device index*/
  char                    location[DVR_MAX_LOCATION_SIZE]; /**< Location of the record file.*/
  int                     block_size;                      /**< playback inject block size*/
  DVR_Bool_t              is_timeshift;                    /**< 0:playback mode, 1 : is timeshift mode*/
  Playback_DeviceHandle_t playback_handle;                 /**< Playback device handle.*/
  DVR_CryptoFunction_t    crypto_fn;                       /**< Crypto function.*/
  void                   *crypto_data;                     /**< Crypto function's user data.*/
  DVR_PlaybackEventFunction_t  event_fn;                   /**< playback event callback function*/
  void                        *event_userdata;             /**< event userdata*/
} DVR_WrapperPlaybackOpenParams_t;

/**
 * Open a new record wrapper.
 * \param[out] rec Return the new record handle.
 * \param params Record open parameters.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_open_record (DVR_WrapperRecord_t *rec, DVR_WrapperRecordOpenParams_t *params);

/**
 * Close an unused record wrapper.
 * \param rec The record handle.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_close_record (DVR_WrapperRecord_t rec);

/**
 * Start recording.
 * \param rec The record handle.
 * \param params Record start parameters.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_start_record (DVR_WrapperRecord_t rec, DVR_WrapperRecordStartParams_t *params);

/**
 * Stop recording..
 * \param rec The record handle.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_stop_record (DVR_WrapperRecord_t rec);

/**
 * Update the recording PIDs.
 * \param rec The record handle.
 * \param params The new PIDs.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_update_record_pids (DVR_WrapperRecord_t rec, DVR_WrapperUpdatePidsParams_t *params);

/**
 * Get the current recording status.
 * \param rec The record handle.
 * \param status The recording status returned.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_get_record_status (DVR_WrapperRecord_t rec, DVR_WrapperRecordStatus_t *status);

/**
 * Open a new playback wrapper handle.
 * \param[out] playback Return the new playback handle.
 * \param params Playback handle open parameters.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_open_playback (DVR_WrapperPlayback_t *playback, DVR_WrapperPlaybackOpenParams_t *params);

/**
 * Close a unused playback handle.
 * \param playback The playback handle to be closed.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_close_playback (DVR_WrapperPlayback_t playback);

/**
 * Start playback.
 * \param playback The playback handle.
 * \param flags Playback flags.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_start_playback (DVR_WrapperPlayback_t playback, DVR_PlaybackFlag_t flags, DVR_PlaybackPids_t *p_pids);

/**
 * Stop playback.
 * \param playback The playback handle.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_stop_playback (DVR_WrapperPlayback_t playback);

/**
 * Pause the playback.
 * \param playback The playback handle.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_pause_playback (DVR_WrapperPlayback_t playback);

/**
 * resume the playback.
 * \param playback The playback handle.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_resume_playback (DVR_WrapperPlayback_t playback);

/**
 * Set the playback speed.
 * \param playback The playback handle.
 * \param speed The new speed.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_set_playback_speed (DVR_WrapperPlayback_t playback, float speed);

/**
 * Seek the current playback position.
 * \param playback The playback handle.
 * \param time_offset The current time in milliseconds.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_seek_playback (DVR_WrapperPlayback_t playback, uint32_t time_offset);

/**
 * Get the current playback status.
 * \param playback The playback handle.
 * \param status The playback status returned.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_get_playback_status (DVR_WrapperPlayback_t playback, DVR_WrapperPlaybackStatus_t *status);

/**
 * Update playback.
 * \param playback The playback handle.
 * \param flags Playback flags.
 * \retval DVR_SUCCESS On success.
 * \return Error code.
 */
int dvr_wrapper_update_playback (DVR_WrapperPlayback_t playback, DVR_PlaybackPids_t *p_pids);


#ifdef __cplusplus
}
#endif

#endif /*DVR_WRAPPER_H_*/

