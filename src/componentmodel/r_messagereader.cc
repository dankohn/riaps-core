//
// Created by istvan on 11/29/18.
//

#include <componentmodel/r_messagereader.h>



MessageReaderArray::MessageReaderArray(zframe_t *msg_frame) : frame_(msg_frame){
    frame_size_ = zframe_size(frame_);
    data_ = zframe_data(frame_);
    capnp_data_ = kj::arrayPtr(reinterpret_cast<const capnp::word*>(data_), frame_size_ / sizeof(capnp::word));
}



MessageReaderArray::~MessageReaderArray() {
    zframe_destroy(&frame_);
}