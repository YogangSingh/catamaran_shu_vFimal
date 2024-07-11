# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from ublox_msgs/EsfSTATUS.msg. Do not edit."""
import codecs
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import ublox_msgs.msg

class EsfSTATUS(genpy.Message):
  _md5sum = "006f2c0e3e6e9239781223dca67e519b"
  _type = "ublox_msgs/EsfSTATUS"
  _has_header = False  # flag to mark the presence of a Header object
  _full_text = """# ESF-STATUS (0x10 0x10)
# External Sensor Fusion (ESF) status information
#
# Supported on UDR/ADR products
#

uint8 CLASS_ID = 16
uint8 MESSAGE_ID = 16

uint32 iTOW                   # GPS time of week of the navigation epoch [ms]
uint8 version                 # Message version (2 for this version)

uint8[7] reserved1            # Reserved

uint8 fusionMode              # Fusion mode:
uint8 FUSION_MODE_INIT = 0        # receiver is initializing some unknown values
                                  # required for doing sensor fusion
uint8 FUSION_MODE_FUSION = 1      # GNSS and sensor data are 
                                  # used for navigation solution computation
uint8 FUSION_MODE_SUSPENDED = 2   # sensor fusion is temporarily disabled 
                                  # due to e.g. invalid sensor data or detected 
                                  # ferry
uint8 FUSION_MODE_DISABLED = 3    # sensor fusion is permanently disabled 
                                  # until receiver reset due e.g. to sensor 
                                  # error

uint8[2] reserved2            # Reserved

uint8 numSens                 # Number of sensors


# Start of repeated block (numSens times)
EsfSTATUS_Sens[] sens
# End of repeated block
================================================================================
MSG: ublox_msgs/EsfSTATUS_Sens
# See Esf-STATUS
#

uint8 sensStatus1   # Sensor status, part 1 (see graphic below)
uint8 sensStatus2   # Sensor status, part 2 (see graphic below)
uint8 freq          # Observation frequency [Hz]
uint8 faults        # Sensor faults (see graphic below)"""
  # Pseudo-constants
  CLASS_ID = 16
  MESSAGE_ID = 16
  FUSION_MODE_INIT = 0
  FUSION_MODE_FUSION = 1
  FUSION_MODE_SUSPENDED = 2
  FUSION_MODE_DISABLED = 3

  __slots__ = ['iTOW','version','reserved1','fusionMode','reserved2','numSens','sens']
  _slot_types = ['uint32','uint8','uint8[7]','uint8','uint8[2]','uint8','ublox_msgs/EsfSTATUS_Sens[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       iTOW,version,reserved1,fusionMode,reserved2,numSens,sens

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(EsfSTATUS, self).__init__(*args, **kwds)
      # message fields cannot be None, assign default values for those that are
      if self.iTOW is None:
        self.iTOW = 0
      if self.version is None:
        self.version = 0
      if self.reserved1 is None:
        self.reserved1 = b'\0'*7
      if self.fusionMode is None:
        self.fusionMode = 0
      if self.reserved2 is None:
        self.reserved2 = b'\0'*2
      if self.numSens is None:
        self.numSens = 0
      if self.sens is None:
        self.sens = []
    else:
      self.iTOW = 0
      self.version = 0
      self.reserved1 = b'\0'*7
      self.fusionMode = 0
      self.reserved2 = b'\0'*2
      self.numSens = 0
      self.sens = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_IB().pack(_x.iTOW, _x.version))
      _x = self.reserved1
      # - if encoded as a list instead, serialize as bytes instead of string
      if type(_x) in [list, tuple]:
        buff.write(_get_struct_7B().pack(*_x))
      else:
        buff.write(_get_struct_7s().pack(_x))
      _x = self.fusionMode
      buff.write(_get_struct_B().pack(_x))
      _x = self.reserved2
      # - if encoded as a list instead, serialize as bytes instead of string
      if type(_x) in [list, tuple]:
        buff.write(_get_struct_2B().pack(*_x))
      else:
        buff.write(_get_struct_2s().pack(_x))
      _x = self.numSens
      buff.write(_get_struct_B().pack(_x))
      length = len(self.sens)
      buff.write(_struct_I.pack(length))
      for val1 in self.sens:
        _x = val1
        buff.write(_get_struct_4B().pack(_x.sensStatus1, _x.sensStatus2, _x.freq, _x.faults))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      if self.sens is None:
        self.sens = None
      end = 0
      _x = self
      start = end
      end += 5
      (_x.iTOW, _x.version,) = _get_struct_IB().unpack(str[start:end])
      start = end
      end += 7
      self.reserved1 = str[start:end]
      start = end
      end += 1
      (self.fusionMode,) = _get_struct_B().unpack(str[start:end])
      start = end
      end += 2
      self.reserved2 = str[start:end]
      start = end
      end += 1
      (self.numSens,) = _get_struct_B().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.sens = []
      for i in range(0, length):
        val1 = ublox_msgs.msg.EsfSTATUS_Sens()
        _x = val1
        start = end
        end += 4
        (_x.sensStatus1, _x.sensStatus2, _x.freq, _x.faults,) = _get_struct_4B().unpack(str[start:end])
        self.sens.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_IB().pack(_x.iTOW, _x.version))
      _x = self.reserved1
      # - if encoded as a list instead, serialize as bytes instead of string
      if type(_x) in [list, tuple]:
        buff.write(_get_struct_7B().pack(*_x))
      else:
        buff.write(_get_struct_7s().pack(_x))
      _x = self.fusionMode
      buff.write(_get_struct_B().pack(_x))
      _x = self.reserved2
      # - if encoded as a list instead, serialize as bytes instead of string
      if type(_x) in [list, tuple]:
        buff.write(_get_struct_2B().pack(*_x))
      else:
        buff.write(_get_struct_2s().pack(_x))
      _x = self.numSens
      buff.write(_get_struct_B().pack(_x))
      length = len(self.sens)
      buff.write(_struct_I.pack(length))
      for val1 in self.sens:
        _x = val1
        buff.write(_get_struct_4B().pack(_x.sensStatus1, _x.sensStatus2, _x.freq, _x.faults))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      if self.sens is None:
        self.sens = None
      end = 0
      _x = self
      start = end
      end += 5
      (_x.iTOW, _x.version,) = _get_struct_IB().unpack(str[start:end])
      start = end
      end += 7
      self.reserved1 = str[start:end]
      start = end
      end += 1
      (self.fusionMode,) = _get_struct_B().unpack(str[start:end])
      start = end
      end += 2
      self.reserved2 = str[start:end]
      start = end
      end += 1
      (self.numSens,) = _get_struct_B().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.sens = []
      for i in range(0, length):
        val1 = ublox_msgs.msg.EsfSTATUS_Sens()
        _x = val1
        start = end
        end += 4
        (_x.sensStatus1, _x.sensStatus2, _x.freq, _x.faults,) = _get_struct_4B().unpack(str[start:end])
        self.sens.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_2B = None
def _get_struct_2B():
    global _struct_2B
    if _struct_2B is None:
        _struct_2B = struct.Struct("<2B")
    return _struct_2B
_struct_2s = None
def _get_struct_2s():
    global _struct_2s
    if _struct_2s is None:
        _struct_2s = struct.Struct("<2s")
    return _struct_2s
_struct_4B = None
def _get_struct_4B():
    global _struct_4B
    if _struct_4B is None:
        _struct_4B = struct.Struct("<4B")
    return _struct_4B
_struct_7B = None
def _get_struct_7B():
    global _struct_7B
    if _struct_7B is None:
        _struct_7B = struct.Struct("<7B")
    return _struct_7B
_struct_7s = None
def _get_struct_7s():
    global _struct_7s
    if _struct_7s is None:
        _struct_7s = struct.Struct("<7s")
    return _struct_7s
_struct_B = None
def _get_struct_B():
    global _struct_B
    if _struct_B is None:
        _struct_B = struct.Struct("<B")
    return _struct_B
_struct_IB = None
def _get_struct_IB():
    global _struct_IB
    if _struct_IB is None:
        _struct_IB = struct.Struct("<IB")
    return _struct_IB
