#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "dvr_types.h"
#include "dvb_utils.h"
#include "dvr_utils.h"

/**
 * Set the demux's input source.
 * \param dmx_idx Demux device's index.
 * \param src The demux's input source.
 * \retval 0 On success.
 * \retval -1 On error.
 */
int dvb_set_demux_source (int dmx_idx, DVB_DemuxSource_t src)
{
	char        node[32];
	struct stat st;
	int         r;

	snprintf(node, sizeof(node), "/sys/class/stb/demux%d_source", dmx_idx);

	r = stat(node, &st);
	if (r == -1) {
		DVR_DEBUG(1, "cannot find \"%s\"", node);
	} else {
		char *val;

		switch (src) {
		case DVB_DEMUX_SOURCE_TS0:
			val = "ts0";
			break;
		case DVB_DEMUX_SOURCE_TS1:
			val = "ts1";
			break;
		case DVB_DEMUX_SOURCE_TS2:
			val = "ts2";
			break;
		case DVB_DEMUX_SOURCE_DMA0:
			val = "hiu";
			break;
		default:
			assert(0);
		}

		r = dvr_file_echo(node, val);
	}

	return r;
}

/**
 * Get the demux's input source.
 * \param dmx_idx Demux device's index.
 * \param point src that demux's input source.
 * \retval 0 On success.
 * \retval -1 On error.
 */
int dvb_get_demux_source (int dmx_idx, DVB_DemuxSource_t *src)
{
    char   node[32] = {0};
    char   buf[32] = {0};
    struct stat st;
    int    r, source_no;

    snprintf(node, sizeof(node), "/sys/class/stb/demux%d_source", dmx_idx);
    r = stat(node, &st);
    if (r == -1) {
        DVR_DEBUG(1, "cannot find \"%s\"", node);
    } else {
       r = dvr_file_read(node, buf, sizeof(buf));
       if (r != -1) {
          if (strncmp(buf, "ts", 2) == 0 && strlen(buf) == 3) {
             sscanf(buf, "ts%d", &source_no);
             switch (source_no)
             {
                 case 0:
                    *src = DVB_DEMUX_SOURCE_TS0;
                    break;
                 case 1:
                    *src = DVB_DEMUX_SOURCE_TS1;
                    break;
                 case 2:
                    *src = DVB_DEMUX_SOURCE_TS2;
                    break;
                 default:
                    DVR_DEBUG(1, "do not support demux source:%s", buf);
                    r = -1;
                    break;
             }
          }else if (strncmp(buf, "hiu", 3) == 0) {
             *src = DVB_DEMUX_SOURCE_DMA0;
          }else {
             r = -1;
          }
          DVR_DEBUG(1, "dvb_get_demux_source \"%s\" :%s", node, buf);
       }
    }

    return r;
}