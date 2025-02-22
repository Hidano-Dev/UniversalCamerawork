# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
# type: ignore

import kaitaistruct
from kaitaistruct import ReadWriteKaitaiStruct, KaitaiStream, BytesIO


if getattr(kaitaistruct, 'API_VERSION', (0, 9)) < (0, 11):
    raise Exception("Incompatible Kaitai Struct Python API: 0.11 or later is required, but you have %s" % (kaitaistruct.__version__))

class Ucapi(ReadWriteKaitaiStruct):
    def __init__(self, _io=None, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self

    def _read(self):
        self.header = Ucapi.Header(self._io, self, self._root)
        self.header._read()
        self.records = []
        for i in range(self.header.record_count):
            _t_records = Ucapi.Record(self._io, self, self._root)
            _t_records._read()
            self.records.append(_t_records)



    def _fetch_instances(self):
        pass
        self.header._fetch_instances()
        for i in range(len(self.records)):
            pass
            self.records[i]._fetch_instances()



    def _write__seq(self, io=None):
        super(Ucapi, self)._write__seq(io)
        self.header._write__seq(self._io)
        for i in range(len(self.records)):
            pass
            self.records[i]._write__seq(self._io)



    def _check(self):
        pass
        if self.header._root != self._root:
            raise kaitaistruct.ConsistencyError(u"header", self.header._root, self._root)
        if self.header._parent != self:
            raise kaitaistruct.ConsistencyError(u"header", self.header._parent, self)
        if (len(self.records) != self.header.record_count):
            raise kaitaistruct.ConsistencyError(u"records", len(self.records), self.header.record_count)
        for i in range(len(self.records)):
            pass
            if self.records[i]._root != self._root:
                raise kaitaistruct.ConsistencyError(u"records", self.records[i]._root, self._root)
            if self.records[i]._parent != self:
                raise kaitaistruct.ConsistencyError(u"records", self.records[i]._parent, self)


    class Header(ReadWriteKaitaiStruct):
        def __init__(self, _io=None, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root

        def _read(self):
            self.signature = self._io.read_bytes(5)
            if not (self.signature == b"\x55\x43\x41\x50\x49"):
                raise kaitaistruct.ValidationNotEqualError(b"\x55\x43\x41\x50\x49", self.signature, self._io, u"/types/header/seq/0")
            self.version_major = self._io.read_u1()
            self.version_minor = self._io.read_u1()
            self.reserved = []
            for i in range(18):
                self.reserved.append(self._io.read_u1())

            self.record_count = self._io.read_u4le()
            self.checksum = self._io.read_u4le()


        def _fetch_instances(self):
            pass
            for i in range(len(self.reserved)):
                pass



        def _write__seq(self, io=None):
            super(Ucapi.Header, self)._write__seq(io)
            self._io.write_bytes(self.signature)
            self._io.write_u1(self.version_major)
            self._io.write_u1(self.version_minor)
            for i in range(len(self.reserved)):
                pass
                self._io.write_u1(self.reserved[i])

            self._io.write_u4le(self.record_count)
            self._io.write_u4le(self.checksum)


        def _check(self):
            pass
            if (len(self.signature) != 5):
                raise kaitaistruct.ConsistencyError(u"signature", len(self.signature), 5)
            if not (self.signature == b"\x55\x43\x41\x50\x49"):
                raise kaitaistruct.ValidationNotEqualError(b"\x55\x43\x41\x50\x49", self.signature, None, u"/types/header/seq/0")
            if (len(self.reserved) != 18):
                raise kaitaistruct.ConsistencyError(u"reserved", len(self.reserved), 18)
            for i in range(len(self.reserved)):
                pass



    class Record(ReadWriteKaitaiStruct):
        def __init__(self, _io=None, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root

        def _read(self):
            self.commands = self._io.read_u2le()
            self.timecode = Ucapi.Timecode(self._io, self, self._root)
            self.timecode._read()
            self.packet_no = self._io.read_u1()
            self.eye_position_right_m = self._io.read_f4le()
            self.eye_position_up_m = self._io.read_f4le()
            self.eye_position_forward_m = self._io.read_f4le()
            self.look_vector_right_m = self._io.read_f4le()
            self.look_vector_up_m = self._io.read_f4le()
            self.look_vector_forward_m = self._io.read_f4le()
            self.up_vector_right_m = self._io.read_f4le()
            self.up_vector_up_m = self._io.read_f4le()
            self.up_vector_forward_m = self._io.read_f4le()
            self.focal_length_mm = self._io.read_f4le()
            self.aspect_ratio = self._io.read_f4le()
            self.focus_distance_m = self._io.read_f4le()
            self.aperture = self._io.read_f4le()
            self.sensor_size_width_mm = self._io.read_f4le()
            self.sensor_size_height_mm = self._io.read_f4le()
            self.near_clip_m = self._io.read_f4le()
            self.far_clip_m = self._io.read_f4le()
            self.lens_shift_horizontal_ratio = self._io.read_f4le()
            self.lens_shift_vertical_ratio = self._io.read_f4le()
            self.lens_distortion_radial_coefficients_k1 = self._io.read_f4le()
            self.lens_distortion_radial_coefficients_k2 = self._io.read_f4le()
            self.lens_distortion_center_point_right_mm = self._io.read_f4le()
            self.lens_distortion_center_point_up_mm = self._io.read_f4le()
            self.reserved = []
            for i in range(29):
                self.reserved.append(self._io.read_u1())



        def _fetch_instances(self):
            pass
            self.timecode._fetch_instances()
            for i in range(len(self.reserved)):
                pass



        def _write__seq(self, io=None):
            super(Ucapi.Record, self)._write__seq(io)
            self._io.write_u2le(self.commands)
            self.timecode._write__seq(self._io)
            self._io.write_u1(self.packet_no)
            self._io.write_f4le(self.eye_position_right_m)
            self._io.write_f4le(self.eye_position_up_m)
            self._io.write_f4le(self.eye_position_forward_m)
            self._io.write_f4le(self.look_vector_right_m)
            self._io.write_f4le(self.look_vector_up_m)
            self._io.write_f4le(self.look_vector_forward_m)
            self._io.write_f4le(self.up_vector_right_m)
            self._io.write_f4le(self.up_vector_up_m)
            self._io.write_f4le(self.up_vector_forward_m)
            self._io.write_f4le(self.focal_length_mm)
            self._io.write_f4le(self.aspect_ratio)
            self._io.write_f4le(self.focus_distance_m)
            self._io.write_f4le(self.aperture)
            self._io.write_f4le(self.sensor_size_width_mm)
            self._io.write_f4le(self.sensor_size_height_mm)
            self._io.write_f4le(self.near_clip_m)
            self._io.write_f4le(self.far_clip_m)
            self._io.write_f4le(self.lens_shift_horizontal_ratio)
            self._io.write_f4le(self.lens_shift_vertical_ratio)
            self._io.write_f4le(self.lens_distortion_radial_coefficients_k1)
            self._io.write_f4le(self.lens_distortion_radial_coefficients_k2)
            self._io.write_f4le(self.lens_distortion_center_point_right_mm)
            self._io.write_f4le(self.lens_distortion_center_point_up_mm)
            for i in range(len(self.reserved)):
                pass
                self._io.write_u1(self.reserved[i])



        def _check(self):
            pass
            if self.timecode._root != self._root:
                raise kaitaistruct.ConsistencyError(u"timecode", self.timecode._root, self._root)
            if self.timecode._parent != self:
                raise kaitaistruct.ConsistencyError(u"timecode", self.timecode._parent, self)
            if (len(self.reserved) != 29):
                raise kaitaistruct.ConsistencyError(u"reserved", len(self.reserved), 29)
            for i in range(len(self.reserved)):
                pass



    class Timecode(ReadWriteKaitaiStruct):
        def __init__(self, _io=None, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root

        def _read(self):
            self.frame_number = self._io.read_bits_int_be(7)
            self.second_number = self._io.read_bits_int_be(6)
            self.minute_number = self._io.read_bits_int_be(6)
            self.hour_number = self._io.read_bits_int_be(5)
            self.reserved = self._io.read_bits_int_be(8)


        def _fetch_instances(self):
            pass


        def _write__seq(self, io=None):
            super(Ucapi.Timecode, self)._write__seq(io)
            self._io.write_bits_int_be(7, self.frame_number)
            self._io.write_bits_int_be(6, self.second_number)
            self._io.write_bits_int_be(6, self.minute_number)
            self._io.write_bits_int_be(5, self.hour_number)
            self._io.write_bits_int_be(8, self.reserved)


        def _check(self):
            pass



