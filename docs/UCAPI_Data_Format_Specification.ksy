meta:
  id: ucapi
  endian: le

seq:
  - id: magic
    type: u2
    doc: Magic number (e.g. 0x55AA)
  - id: version
    type: u2
    doc: Packet format version (e.g. 0x0002)
  - id: num_payload
    type: u2
    doc: Number of records in this packet
  - id: payload_length
    type: u2
    doc: Total length of payload in bytes (num_payload × payload_length)
  - id: crc16
    type: u2
    doc: CRC-16 checksum of payload[]
  - id: payload
    size: num_payload * payload_length
    type: record
    repeat: expr
    repeat-expr: num_payload
    doc: Array of UCAPI records

types:
  timecode:
    seq:
      - id: frame_number
        type: b7
      - id: second_number
        type: b6
      - id: minute_number
        type: b6
      - id: hour_number
        type: b5
      - id: reserved
        type: b8

  record:
    seq:
      - id: camera_no
        type: u4
      - id: commands
        type: u2
      - id: timecode
        type: timecode
      - id: packet_no
        type: u1
      - id: eye_position_right_m
        type: f4
      - id: eye_position_up_m
        type: f4
      - id: eye_position_forward_m
        type: f4
      - id: look_vector_right_m
        type: f4
      - id: look_vector_up_m
        type: f4
      - id: look_vector_forward_m
        type: f4
      - id: up_vector_right_m
        type: f4
      - id: up_vector_up_m
        type: f4
      - id: up_vector_forward_m
        type: f4
      - id: focal_length_mm
        type: f4
      - id: aspect_ratio
        type: f4
      - id: focus_distance_m
        type: f4
      - id: aperture
        type: f4
      - id: sensor_size_width_mm
        type: f4
      - id: sensor_size_height_mm
        type: f4
      - id: near_clip_m
        type: f4
      - id: far_clip_m
        type: f4
      - id: lens_shift_horizontal_ratio
        type: f4
      - id: lens_shift_vertical_ratio
        type: f4
      - id: lens_distortion_radial_coefficients_k1
        type: f4
      - id: lens_distortion_radial_coefficients_k2
        type: f4
      - id: lens_distortion_center_point_right_mm
        type: f4
      - id: lens_distortion_center_point_up_mm
        type: f4
      - id: reserved
        type: u1
        repeat: expr
        repeat-expr: 25
