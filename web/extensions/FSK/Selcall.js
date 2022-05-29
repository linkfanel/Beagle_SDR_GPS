// Copyright (c) 2022 John Seamons, ZL/KF6VO

function Selcall(init, output_cb) {
   var t = this;

   console.log('FSK encoder: Selcall');

   t.dbg = 0;
   t.test_msgs = 0;
   t.dump_non_std_len = 1;
   t.dump_no_eos = 0;
   
   t.start_bit = 0;
   t.seq = 0;
   t.MSG_LEN_MIN = 30;
   t.MSG_LEN_MAX = 80;
   t.synced = 0;

   t.sym_s = [    // sync & format
   //   xx0    xx1    xx2    xx3    xx4    xx5    xx6    xx7    xx8    xx9
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 00x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 01x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 02x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 03x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 04x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 05x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 06x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 07x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 08x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ', '   ',   // 09x
      'RTN', '   ', '   ', '   ', 'Pr0', 'Pr1', 'Pr2', 'Pr3', 'Pr4', 'Pr5',   // 10x
      '   ', '   ', '   ', '   ', '   ', '   ', '   ', 'ARQ', '   ', '   ',   // 11x
      'SEL', '   ', 'ABQ', 'SMA', '   ', 'Pdx', '  *', 'EOS'                  // 12x
   ];
   
   t.DX  = 125;
   t.RX5 = 109;
   
   t.FMT_GEO_AREA = 102;
   t.FMT_DISTRESS = 112;
   t.FMT_COMMON_INTEREST = 114;
   t.FMT_ALL_SHIPS = 116;
   t.FMT_INDIV_STA = 120;
   t.FMT_RESV = 121;
   t.FMT_IS_SEMI_AUTO = 123;

   t.CAT_ROUTINE = 100;
   t.CAT_SAFETY = 108;
   t.CAT_URGENCY = 110;
   t.CAT_DISTRESS = 112;
   
   t.CMD1_FM_CALL = 100;
   t.CMD1_FM_DUPLEX_CALL = 101;
   t.CMD1_POLLING = 103;
   t.CMD1_UNABLE_COMPLY = 104;
   t.CMD1_END_OF_CALL = 105;
   t.CMD1_F1B_DATA = 106;
   t.CMD1_J3E_RT = 109;
   t.CMD1_DISTRESS_ACK = 110;
   t.CMD1_DISTRESS_ALERT_RELAY = 112;
   t.CMD1_F1B_FEC = 113;
   t.CMD1_F1B_ARQ = 115;
   t.CMD1_TEST = 118;
   t.CMD1_POSITION = 121;
   t.CMD1_NOP = 126;

   t.CMD2_UNABLE_FIRST = 100;
   t.CMD2_BUSY = 102;
   t.CMD2_UNABLE_LAST = 109;
   t.CMD2_NON_CONFLICT = 110;
   t.CMD2_MED_TRANSPORTS = 111;
   t.CMD2_NOP = 126;
   
   t.ARQ = 117;
   t.ABQ = 122;
   t.EOS = 127;
   
   t.NOP = 126;
   
   t.pos_s = [
   /*
      // 6-digit A1|A2|A3 => I1|I2|I3
      'dx', 'rx5', 'dx', 'rx4', 'dx', 'rx3', 'dx', 'rx2', 'dx', 'rx1', 'dx', 'rx0',
      'fmt', 'fmt', 'A1', 'fmt', 'A2', 'fmt', 'A3', 'A1', 'cat', 'A2',
      'I1', 'A3', 'I2', 'cat', 'I3', 'I1',
      'eos', 'I2', 'eos', 'I3', 'eos', 'eos'
   */

      // 4-digit B1|B2 => A1|A2
      'dx', 'rx5', 'dx', 'rx4', 'dx', 'rx3', 'dx', 'rx2', 'dx', 'rx1', 'dx', 'rx0',
      'fmt', 'fmt', 'B1', 'fmt', 'B2', 'fmt', 'cat', 'B1',
      'A1', 'B2', 'A2', 'cat',
      'eos', 'A1', 'eos', 'A2', 'eos', 'eos'
   ];
   
   // symbolic position lookup
   t.pos = {};
   t.pos_n = {};
   t.pos_s.forEach(function(pos_s, i) {
      pos_s = pos_s.trim();
      if (!isDefined(t.pos_n[pos_s])) t.pos_n[pos_s] = 0;
      t.pos[pos_s +'-'+ t.pos_n[pos_s].toString()] = i;
      t.pos_n[pos_s]++;
   });
   if (t.dbg) console.log(t.pos);
   
   var edc = function(v) {
      var bc = kiwi_bitCount(v ^ 0x7f);   // count # of zeros
      return kiwi_bitReverse(bc, 3);
   };

   t.DX_w = (edc(t.DX) << 7) | t.DX;
   t.svec = [];
   var rx = t.RX5;
   for (var i = 0; i < 12; i++) {
      t.svec[i] = (i&1)? ((edc(rx) << 7) | rx) : t.DX_w;
      if (i&1) rx--;
   }
   if (t.dbg) console.log(t.svec);
}

Selcall.prototype.sym_by_sym_name = function(sym_name) {
   var t = this;
   var pos = t.pos[sym_name];
   //console.log(sym_name +'=@'+ pos +'='+ t.syms[pos]);
   return { /*name:sym_name,*/ pos:pos, sym:t.syms[pos] };
}
   
Selcall.prototype.output_msg = function(s) {
   var t = this;
   //toUTCString().substr(5,20)
   return ((new Date()).toUTCString().substr(17,8) +'Z '+ (ext_get_freq()/1e3).toFixed(2) +' '+ s +'\n');
}
   
Selcall.prototype.code_to_char = function(code) {
   
   var t = this;
   if (code < 0 || code > 127) return '???';
   var sym = t.sym_s[code];
   //if (sym == '   ') return '['+ code +']';
   return sym;
}

Selcall.prototype.reset = function() {
   var t = this;
   t.syncA = t.syncB = t.syncC = t.syncD = t.syncE = 0;
}

Selcall.prototype.get_nbits = function() {
   return 10;
}

Selcall.prototype.get_msb = function() {
   return 0x200;
}

Selcall.prototype.check_bits = function() {
   return false;
}

Selcall.prototype.search_sync = function(bit) {
   var t = this;
   var cin, cout;

   cin = bit;
   cout = t.syncA & 1;
   t.syncA = (t.syncA >> 1) & 0x3ff;
   t.syncA |= cin? 0x200:0;

   cin = cout;
   cout = t.syncB & 1;
   t.syncB = (t.syncB >> 1) & 0x3ff;
   t.syncB |= cin? 0x200:0;

   cin = cout;
   cout = t.syncC & 1;
   t.syncC = (t.syncC >> 1) & 0x3ff;
   t.syncC |= cin? 0x200:0;

   cin = cout;
   cout = t.syncD & 1;
   t.syncD = (t.syncD >> 1) & 0x3ff;
   t.syncD |= cin? 0x200:0;

   cin = cout;
   cout = t.syncE & 1;
   t.syncE = (t.syncE >> 1) & 0x3ff;
   t.syncE |= cin? 0x200:0;
   
   if (0) {
      console.log(bit +' '+
         t.syncA.toString(2).leadingZeros(10) + paren(t.syncA.toHex(-3)) +'|'+
         t.syncB.toString(2).leadingZeros(10) + paren(t.syncB.toHex(-3)) +'|'+
         t.syncC.toString(2).leadingZeros(10) + paren(t.syncC.toHex(-3)) +'|'+
         t.syncD.toString(2).leadingZeros(10) + paren(t.syncD.toHex(-3)) +'|'+
         t.syncE.toString(2).leadingZeros(10) + paren(t.syncE.toHex(-3)));
      console.log('> '+
         t.svec[1].toString(2).leadingZeros(10) + paren(t.svec[1].toHex(-3)) +'|'+
         (0+0).toString(2).leadingZeros(10) + paren((0+0).toHex(-3)) +'|'+
         t.svec[3].toString(2).leadingZeros(10) + paren(t.svec[3].toHex(-3)) +'|'+
         (0+0).toString(2).leadingZeros(10) + paren((0+0).toHex(-3)) +'|'+
         t.svec[5].toString(2).leadingZeros(10) + paren(t.svec[5].toHex(-3)));
   }

   for (var i = 0; i <= (12-3); i++) {
      if (t.syncC == t.svec[i] && t.syncB == t.svec[i+1] && t.syncA == t.svec[i+2]) {
         if (1 || t.dbg) console.log('SYNC-DRD-'+ i);
         t.seq = i+3;
         t.full_syms = [];
         t.syms = [];
         t.bc_err = [];
         t.parity_errors = 0;
         t.FEC_errors = 0;
         t.synced = 1;
         return true;
      }
   }
   for (var i = 1; i <= 7; i += 2) {
      if (t.syncE == t.svec[i] && t.syncC == t.svec[i+2] && t.syncA == t.svec[i+4]) {
         if (1 || t.dbg) console.log('SYNC-RRR-'+ i);
         t.seq = i+5;
         t.full_syms = [];
         t.syms = [];
         t.bc_err = [];
         t.parity_errors = 0;
         t.FEC_errors = 0;
         t.synced = 1;
         return true;
      }
   }
   
   t.synced = 0;
   return false;
}

Selcall.prototype.process_char = function(_code, fixed_start, cb, show_errs) {
   var t = this;
   if (!t.synced) return { resync:0 };

   t.full_syms[t.seq] = _code;
   var bc_rev = (_code >> 7) & 7;
   var code = _code & 0x7f;
   t.syms[t.seq] = code;
   var pos_s = t.pos_s[t.seq];
   pos_s = pos_s || 'unk';
   var chr = t.code_to_char(code);

   // verify #zeros count
   var bc_ck = kiwi_bitReverse(bc_rev, 3);
   var bc_data = kiwi_bitCount(code ^ 0x7f);
   var pe = (bc_ck != bc_data);
   var zc = pe? (' Zck='+ bc_ck +' Zdata='+ bc_data) : '';
   t.bc_err[t.seq] = pe;
   if (pe) t.parity_errors++;

   if (t.dbg) console.log(t.seq.leadingZeros(2) +' '+ pos_s.fieldWidth(6) +': '+ chr +' '+ code.fieldWidth(3) +'.'+
      ' '+ bc_rev.toString(2).leadingZeros(3) +' '+ code.toString(2).leadingZeros(7) + zc);
   t.seq++;

   // detect variable length EOS
   var eos = false;

   if (t.seq >= t.MSG_LEN_MIN) {
      var eos_n = 0, s;
      
      var eos_ck = function(eos_sym) {
         var eos_n = 0;
         if (!t.bc_err[t.seq-1] && t.syms[t.seq-1] == eos_sym) eos_n++;
         if (!t.bc_err[t.seq-2] && t.syms[t.seq-2] == eos_sym) eos_n++;
         if (!t.bc_err[t.seq-4] && t.syms[t.seq-4] == eos_sym) eos_n++;
         if (!t.bc_err[t.seq-6] && t.syms[t.seq-6] == eos_sym) eos_n++;
         if (eos_n >= 3) console.log('eos_n('+ eos_sym +')='+ eos_n);
         return (eos_n >= 3);
      }
      
      var color = function(color, s) {
         return (color + s + ansi.NORM);
      };

      if (eos_ck(t.EOS) || eos_ck(t.ARQ) || eos_ck(t.ABQ)) eos = true;
      if (eos || t.seq > t.MSG_LEN_MAX) {
         var dump = 0;
         if (eos) {
            if (t.seq != t.MSG_LEN_MIN) {
               if (show_errs) cb(t.output_msg(color(ansi.BLUE, 'non-std len='+ t.seq)));
               console.log('$$ non-std len='+ t.seq);
               dump = t.dump_non_std_len;
            }
            //cb(t.process_msg(show_errs));
         } else {
            var pe = t.parity_errors? (' '+ color(ansi.BLUE, (t.parity_errors +' PE'))) : '';
            if (show_errs) cb(t.output_msg(color(ansi.BLUE, 'no EOS') + pe));
            console.log('$$ no EOS pe='+ t.parity_errors);
            dump = t.dump_no_eos;
         }

         if (dump && dbgUs) {
            var dump_line = 1;
            var fec_err = false;
            var color_n = -1, color;
            var prev_len = 0;
            var s = '';

            for (var i = 0; i < t.seq; i++) {
               var _code = t.full_syms[i];
               if (isUndefined(_code)) _code = 119;   // sync
               var code = _code & 0x7f;
               var pos_s = t.pos_s[i];
               pos_s = pos_s || 'unk';
               
               if (dump_line) {
                  if (i & 1 || i < 12) continue;      // skip FEC and phasing
                  if (t.bc_err[i] && i+5 < t.seq) {   // apply FEC
                     if (!t.bc_err[i+5]) {
                        code = t.full_syms[i+5] & 0x7f;
                     } else {
                        code = 'FEC';
                        fec_err = true;
                     }
                  }
                  //s += pos_s +'|'+ code +' ';
                  var code_s = (code <= 99)? code.leadingZeros(2) : code.toString();
                  if (prev_len != code_s.length) {
                     color_n = (color_n + 1) % ansi.rolling_n;
                     color = ansi[ansi.rolling[color_n]];
                     prev_len = code_s.length;
                  }
                  s += color + code_s + ansi.NORM +' ';
               } else {
                  var chr = t.code_to_char(code);
                  var bc_rev = (_code >> 7) & 7;
                  var zc = '';
                  if (t.bc_err[i]) {
                     var bc_ck = kiwi_bitReverse(bc_rev, 3);
                     var bc_data = kiwi_bitCount(code ^ 0x7f);
                     var pe = (bc_ck != bc_data);
                     zc = ' Zck='+ bc_ck +' Zdata='+ bc_data;
                  }
                  console.log(i.leadingZeros(2) +' '+ pos_s.fieldWidth(6) +': '+ chr +' '+ code.fieldWidth(3) +'.'+
                     ' '+ bc_rev.toString(2).leadingZeros(3) +' '+ code.toString(2).leadingZeros(7) + zc);
               }
            }

            if (dump_line) {
               //console.log(s);
               cb(s +'\n');
            }
         }

         t.synced = 0;
         return { resync:1 };
      }
   }
   
   return { resync:0 };
}