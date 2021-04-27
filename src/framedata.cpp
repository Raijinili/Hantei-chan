// .HA6 loader:
//
// Hantei6 frame data load and storage.

#include "framedata.h"
#include "misc.h"
#include <cstring>
#include <cassert>
#include <sstream>
#include <iomanip>

Sequence::Sequence():
empty(true),
initialized(false)
{}

struct TempInfo {
	Sequence	*seq;
	
	unsigned int	cur_hitbox;
	unsigned int	cur_AT;
	unsigned int	cur_AS;
	unsigned int	cur_EF;
	unsigned int	cur_IF;
};

// local recursive frame data loader

static unsigned int *fd_frame_AT_load(unsigned int *data, const unsigned int *data_end,
				Frame_AT *AT) {
	AT->active = 1;
	AT->guard_flags = 0;
	AT->proration = 0;
	AT->proration_type = 0;
	AT->damage = 0;
	AT->red_damage = 0;
	AT->circuit_gain = 0;
	
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "ATGD", 4)) {
			AT->guard_flags = data[0];
			++data;
		} else if (!memcmp(buf, "ATHS", 4)) {
			AT->proration = data[0];
			++data;
		} else if (!memcmp(buf, "ATVV", 4)) {
			short *d = (short *)data;
			AT->red_damage = d[0];
			AT->damage = d[1];
			AT->dmg_unknown = d[2];
			AT->circuit_gain = d[3];
			data += 2;
		} else if (!memcmp(buf, "ATHT", 4)) {
			AT->proration_type = data[0];
			data += 1;
		} else if (!memcmp(buf, "ATED", 4)) {
			break;
		}
		

		
		// unhandled:
		// ATGV(V) - ?
		// ATHV(V) - ?
		// ATHE(2) - ?
		// ATF1(1) - stun?
		// ATKK(1) - ?
		// ATNG(1) - ?
		// ATUH(1) - ?
		// ATGN(1) - ?
		// ATBT(1) - ?
		// ATSN(1) - ?
		// ATSU(1) - ?
		// ATSP(1) - ?
		// (no others)
	}
	
	return data;
}

static unsigned int *fd_frame_AS_load(unsigned int *data, const unsigned int *data_end,
				Frame_AS *AS) {
	AS->speed_flags = 0;
	AS->ASMV = -1;
	
	AS->stand_state = 0;
	
	AS->cancel_flags = 0;
		
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "ASV0", 4)) {
			int *d = (int *)data;
			AS->speed_flags = d[0];
			AS->speed_horz = d[1];
			AS->speed_vert = d[2];
			AS->accel_horz = d[3];
			AS->accel_vert = d[4];
			data += 5;
		} else if (!memcmp(buf, "ASVX", 4)) {
			AS->speed_flags = 0x33; // ?
			AS->speed_horz = 0;
			AS->speed_vert = 0;
			AS->accel_horz = 0;
			AS->accel_vert = 0;
		} else if (!memcmp(buf, "ASMV", 4)) {
			AS->ASMV = data[0];
			++data;
		} else if (!memcmp(buf, "ASS1", 4)) {
			AS->stand_state = 1; // airborne
		} else if (!memcmp(buf, "ASS2", 4)) {
			AS->stand_state = 2; // crouching
		} else if (!memcmp(buf, "ASCN", 4)) {
			AS->cancel_flags |= 1;
		} else if (!memcmp(buf, "ASCS", 4)) {
			AS->cancel_flags |= 2;
		} else if (!memcmp(buf, "ASCT", 4)) {
			AS->cancel_flags |= 4;
			/*
		} else if (!memcmp(buf, "AST0", 4)) {
			float *d = (float *)(data+5);
			printf("AST0: %d %d %d %d %d %1.10f %d\n",
				data[0], data[1], data[2], data[3],
				data[4], *d, data[6]);
			
			data += 7;
			 */
		} else if (!memcmp(buf, "ASED", 4)) {
			break;
		}
		
		// unhandled:
		// ASAA(1) - ???
		// ASMX(1) - ?
		// ASF0(1) - ?
		// ASF1(1) - ?
		// ASYS(1) - ?
		// AST0(7) - ??? Kohaku/Necos/Nero/Roa/Ryougi/Warakia only
		// (no others)
	}
	
	return data;
}

static unsigned int *fd_frame_EF_load(unsigned int *data, const unsigned int *data_end,
				Frame_EF *EF) {
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "EFTP", 4)) {
			EF->command = data[0];
			++data;
		} else if (!memcmp(buf, "EFNO", 4)) {
			EF->parameter = data[0];
			++data;
		} else if (!memcmp(buf, "EFPR", 4)) {
			int count = data[0];
			if (count <= 12) {
				for (int i = 0; i < count; ++i) {
					EF->values[i] = data[i+1];
				}
			} else {
				//printf("EFPR %d ???\n",count);
			}
			data += count + 1;
		} else if (!memcmp(buf, "EFED", 4)) {
			break;
		}
	}
	
	return data;
}

static unsigned int *fd_frame_IF_load(unsigned int *data, const unsigned int *data_end,
				Frame_IF *IF) {
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "IFTP", 4)) {
			IF->command = data[0];
			++data;
		} else if (!memcmp(buf, "IFPR", 4)) {
			int count = data[0];
			if (count <= 12) {
				for (int i = 0; i < count; ++i) {
					IF->values[i] = data[i+1];
				}
			} else {
				//printf("IFPR %d ???\n",count);
			}
			data += count + 1;
		} else if (!memcmp(buf, "IFED", 4)) {
			break;
		}
	}
	
	return data;
}

static unsigned int *fd_frame_AF_load(unsigned int *data, const unsigned int *data_end,
				Frame *frame) {
	frame->AF.active = 1;
	frame->AF.frame = -1;
	frame->AF.frame_unk = -1;
	frame->AF.offset_x = 0;
	frame->AF.offset_y = 8;
	frame->AF.duration = 1;
	frame->AF.AFF = -1;
	frame->AF.blend_mode = 0;
	frame->AF.alpha = 255;
	frame->AF.red = 255;
	frame->AF.green = 255;
	frame->AF.blue = 255;
	frame->AF.z_rotation = 0.0;
	frame->AF.x_rotation = 0.0;
	frame->AF.y_rotation = 0.0;
	frame->AF.has_zoom = 0;
	frame->AF.zoom_x = 1.0;
	frame->AF.zoom_y = 1.0;
	frame->AF.AFJP = -1;
	
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "AFGP", 4)) {
			int *dt = (int *)data;
			frame->AF.frame = dt[1];
			frame->AF.frame_unk = dt[0];
			data += 2;
		} else if (!memcmp(buf, "AFOF", 4)) {
			int *dt = (int *)data;
			frame->AF.offset_y = dt[1];
			frame->AF.offset_x = dt[0];
			data += 2;
		} else if (!memcmp(buf, "AFD", 3)) {
			char t = ((char *)buf)[3];
			if (t >= '0' && t <= '9') {
				frame->AF.duration = t - '0';
			} else if (t == 'L') {
				frame->AF.duration = data[0];
				++data;
			}
		} else if (!memcmp(buf, "AFY", 3)) {
			// yeah i don't know.
			// 7/8/9/X/1/2/3 -> 7/8/9/10/11/12/13
			// other numbers are not used.
			char t = ((char *)buf)[3];
			if (t >= '0' && t <= '9') {
				int v = (t - '0');
				if (v < 4) {
					v += 10;
				}
				frame->AF.offset_y = v;
			} else if (t == 'X') {
				frame->AF.offset_y = 10;
			}
		} else if (!memcmp(buf, "AFF", 3)) {
			char t = ((char *)buf)[3];
			if (t >= '0' && t <= '9') {
				frame->AF.AFF = t - '0';
			} else if (t == 'X') {
				frame->AF.AFF = data[0];
				++data;
			}
		} else if (!memcmp(buf, "AFAL", 4)) {
			frame->AF.blend_mode = data[0];
			frame->AF.alpha = data[1];
			data += 2;
		} else if (!memcmp(buf, "AFRG", 4)) {
			frame->AF.red = data[0];
			frame->AF.green = data[1];
			frame->AF.blue = data[2];
			data += 3;
		} else if (!memcmp(buf, "AFAZ", 4)) {
			frame->AF.z_rotation = *(float *)data;
			
			++data;
		} else if (!memcmp(buf, "AFAY", 4)) {
			frame->AF.y_rotation = *(float *)data;
			
			++data;
		} else if (!memcmp(buf, "AFAX", 4)) {
			frame->AF.x_rotation = *(float *)data;
			
			++data;
		} else if (!memcmp(buf, "AFZM", 4)) {
			frame->AF.has_zoom = 1;
			frame->AF.zoom_x = ((float *)data)[0];
			frame->AF.zoom_y = ((float *)data)[1];
			
			data += 2;
		} else if (!memcmp(buf, "AFJP", 4)) {
			frame->AF.AFJP = data[0];
			++data;
		} else if (!memcmp(buf, "AFED", 4)) {
			break;
		}
		
		// unhandled:
		// AFTN(2)
		// AFRT(1)
		// AFHK(1)
		// AFPR(1)
		// AFCT(1)
		// AFJP(1)
		// AFLP(1)
		// AFJC(1)
	}
	
	return data;
}

static unsigned int *fd_frame_load(unsigned int *data, const unsigned int *data_end,
				Frame *frame, TempInfo *info) {
	memset(frame, 0, sizeof(Frame));
	
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "HRNM", 4) || !memcmp(buf, "HRAT", 4)) {
			// read hitbox or attackbox
			unsigned int n = data[0];
			if (!memcmp(buf, "HRAT", 4)) {
				n += 25;
			}
			if (n <= 32 && info->cur_hitbox < info->seq->hitboxes.size()) {
				Hitbox *d = &info->seq->hitboxes[info->cur_hitbox];
				++info->cur_hitbox;
				
				frame->hitboxes[n] = d;
			
				int *dt = (int *)data;
			
				d->x1 = (dt[1]);
				d->y1 = (dt[2]);
				d->x2 = (dt[3]);
				d->y2 = (dt[4]);
			}
			
			data += 5;
		} else if (!memcmp(buf, "HRNS", 4) || !memcmp(buf, "HRAS", 4)) {
			// read hitbox reference
			unsigned int location = data[0];
			unsigned int source = data[1];
			
			if (!memcmp(buf, "HRAS", 4)) {
				location += 25;
			}
			
			if (location <= 32 && source < info->cur_hitbox) {
				frame->hitboxes[location] = &info->seq->hitboxes[source];
			} 
			
			data += 2;
		} else if (!memcmp(buf, "ATST", 4)) {
			// start attack block
			if (info->cur_AT < info->seq->AT.size()) {
				frame->AT = &info->seq->AT[info->cur_AT];
				++info->cur_AT;

				data = fd_frame_AT_load(data, data_end, frame->AT);
			}
		} else if (!memcmp(buf, "ASST", 4)) {
			// start state block
			if (info->cur_AS < info->seq->AS.size()) {
				frame->AS = &info->seq->AS[info->cur_AS];
				++info->cur_AS;
				
				data = fd_frame_AS_load(data, data_end, frame->AS);
			}
		} else if (!memcmp(buf, "ASSM", 4)) {
			// reference previous state block
			unsigned int value = data[0];
			++data;
			
			if (value < info->cur_AS) {
				frame->AS = &info->seq->AS[value];
			}
		} else if (!memcmp(buf, "AFST", 4)) {
			// start render frame block
			data = fd_frame_AF_load(data, data_end, frame);
		} else if (!memcmp(buf, "EFST", 4)) {
			// start effect block
			int n = data[0];
			++data;
			if (n < 8 && info->cur_EF < info->seq->EF.size()) {
				frame->EF[n] = &info->seq->EF[info->cur_EF];
				++info->cur_EF;
				
				data = fd_frame_EF_load(data, data_end, frame->EF[n]);
			}
		} else if (!memcmp(buf, "IFST", 4)) {
			// start cancel block
			int n = data[0];
			++data;
			if (n < 8 && info->cur_IF < info->seq->IF.size()) {
				frame->IF[n] = &info->seq->IF[info->cur_IF];
				++info->cur_IF;
				
				data = fd_frame_IF_load(data, data_end, frame->IF[n]);
			}
		} else if (!memcmp(buf, "FEND", 4)) {
			break;
		}
		
		// unhandled:
		// FSNA(1) ?
		// FSNH(1) ?
		// FSNE(1) ?
		// FSNI(1) ?
		// (no others)
	}
	
	return data;
}

static unsigned int *fd_sequence_load(unsigned int *data, const unsigned int *data_end,
				Sequence *seq) {
	TempInfo temp_info;
	unsigned int frame_it = 0, nframes = 0;
	
	temp_info.seq = seq;
	temp_info.cur_hitbox = 0;
	temp_info.cur_AT = 0;
	temp_info.cur_AS = 0;
	temp_info.cur_EF = 0;
	temp_info.cur_IF = 0;
	
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "PTCN", 4)) {
			data = (unsigned int *)(((unsigned char *)data)+5);
		} else if (!memcmp(buf, "PSTS", 4)) {
			++data;
		} else if (!memcmp(buf, "PLVL", 4)) {
			++data;
		} else if (!memcmp(buf, "PFLG", 4)) {
			++data;
		} else if (!memcmp(buf, "PDST", 4)) {
			// PDST is only used on G_CHAOS
			// seems to be an older format with vague
			// allocations. On-demand maybe.
			++data;
		} else if (!memcmp(buf, "PTT2", 4)) {
			// variable-length sequence title
			unsigned int len = data[0];
			if (len < 64) {
				char str[65];
				memcpy(str, data+1, len);
				str[len] = '\0';
				
				seq->name = str;
				seq->name = sj2utf8(seq->name);
			}
			
			data += 1 + ((len+3)/4);
		} else if (!memcmp(buf, "PTIT", 4)) {
			//TODO: verify and remove
			assert(1);
			// fixed-length sequence title
			char str[33];
			memcpy(str, data, 32);
			str[32] = '\0';
			
			
			seq->name = str;
			seq->name = sj2utf8(seq->name);
			
			data += 8;
		} else if (!memcmp(buf, "PDS2", 4)) {
			// this is an allocation call
			// format:
			// data[0] = byte count // Always 32?
			// data[1] = frame count
			// data[2] = hitbox count
			// data[3] = EF count
			// data[4] = IF count
			// data[5] = AT count
			// data[6] = unused
			// data[7] = AS count
			// from this we can tell what's referenced
			// and what's static for each frame. yay!
			if (data[0] == 32) {

				//Ended up a lot more simple huh
				seq->frames.resize(data[1]);
				seq->hitboxes.resize(data[2]);
				seq->EF.resize(data[3]);
				seq->IF.resize(data[4]);
				seq->AT.resize(data[5]);
				seq->AS.resize(data[7]);

				nframes = data[1];

				seq->initialized = 1;
			}
			data += 1 + (data[0]/4);
		} else if (!memcmp(buf, "FSTR", 4)) {
			if (seq->initialized && frame_it < nframes) {
				Frame *frame = &seq->frames[frame_it];
				data = fd_frame_load(data, data_end, frame, &temp_info);
			
				++frame_it;
			}
		} else if (!memcmp(buf, "PEND", 4)) {
			break;
		}
	}
	
	if(seq->initialized)
		assert(frame_it == nframes);
	
	return data;
}

static unsigned int *fd_main_load(unsigned int *data, const unsigned int *data_end,
				Sequence *sequences,
				unsigned int nsequences) {
	while (data < data_end) {
		unsigned int *buf = data;
		++data;
		
		if (!memcmp(buf, "PSTR", 4)) {
			unsigned int seq_id = *data;
			++data;
			
			// make sure there's actually something here.
			if (memcmp(data, "PEND", 4)) {
				if (seq_id < nsequences) {
					sequences[seq_id].empty = false;
					data = fd_sequence_load(data, data_end, &sequences[seq_id]);
				}
			} else {
				++data;
			}
		} else if (!memcmp(buf, "_END", 4)) {
			break;
		}
	}
	
	return data;
}

bool FrameData::load(const char *filename) {
	// allow loading over existing data
	
	char *data;
	unsigned int size;
	
	if (!ReadInMem(filename, data, size)) {
		return 0;
	}
	
	// verify header
	if (memcmp(data, "Hantei6DataFile", 15)) {
		delete[] data;
		
		return 0;
	}
	
	// initialize the root
	unsigned int *d = (unsigned int *)(data + 0x20);
	unsigned int *d_end = (unsigned int *)(data + size);
	if (memcmp(d, "_STR", 4)) {
		delete[] data;
		return 0;
	}
	
	unsigned int sequence_count = d[1];
	
	Free();
	m_nsequences = sequence_count;
	m_sequences = new Sequence[m_nsequences];

	d += 2;	
	// parse and recursively store data
	d = fd_main_load(d, d_end, m_sequences, m_nsequences);
	
	// cleanup and finish
	delete[] data;
	
	m_loaded = 1;
	
	return 1;
}

static char *split_line(char **data) {
	char *start = *data;
	
	while (*start == ' ' || *start == '\t') {
		++start;
	}
	
	char *p = start;
	while (*p && *p != '\n' && *p != '\r') ++p;
	
	if (*p) {
		*p++ = '\0';
	}
	*data = p;
	
	return start;
}

void FrameData::Free() {
	delete[] m_sequences;
	m_sequences = nullptr;
	m_nsequences = 0;
	m_loaded = 0;
}

int FrameData::get_sequence_count() {
	if (!m_loaded) {
		return 0;
	}
	return m_nsequences;
}

Sequence* FrameData::get_sequence(int n) {
	if (!m_loaded) {
		return 0;
	}
	
	if (n < 0 || (unsigned int)n >= m_nsequences) {
		return 0;
	}
	
	return &m_sequences[n];
}

std::string FrameData::GetDecoratedName(int n)
{
		std::stringstream ss;
		ss.flags(std::ios_base::right);
		
		ss << std::setfill('0') << std::setw(3) << n;
		if(!m_sequences[n].empty)
		{
			ss << " ";
			if(m_sequences[n].name.empty())
			{
				if(m_sequences[n].frames.empty())
					ss << u8"〇";
				else
					ss << u8"Untitled";
			}
			else
				ss << m_sequences[n].name;
			
		}

		return ss.str();
}

FrameData::FrameData() {
	m_sequences = nullptr;
	
	m_nsequences = 0;
	
	m_loaded = 0;
}

FrameData::~FrameData() {
	Free();
}
