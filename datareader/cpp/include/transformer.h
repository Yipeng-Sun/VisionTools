/**
 * Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef DATAREADER_CPP_INCLUDE_TRANSFORMER_H_
#define DATAREADER_CPP_INCLUDE_TRANSFORMER_H_

#include <map>
#include <string>
#include <vector>

namespace vistool {

typedef std::map<std::string, std::string> kv_conf_t;
typedef std::map<std::string, std::string>::iterator kv_conf_iter_t;
typedef std::map<std::string, std::string>::const_iterator kv_conf_const_iter_t;

struct transformer_input_data_t {
  transformer_input_data_t() : id(0) {}
  unsigned int id;
  std::vector<char> data;
  std::string label;
};

struct transformer_output_data_t {
  transformer_output_data_t() : id(0), err_no(0) {}
  unsigned int id;
  int err_no;
  std::string err_msg;

  std::string label;
  std::vector<int> shape;
  std::string data;
};

enum TRANSFORMER_ERR_CODE_TYPE {
  TRANS_ERR_OK = 0,
  TRANS_ERR_NO_OUTPUT = 1,
  TRANS_ERR_LOGICERROR_EXCEPTION = 1,
  TRANS_ERR_STOPPED = 1000,
  TRANS_ERR_INVALID_OP_NAME = 1001,
  TRANS_ERR_RESIZE_NO_INPUT = 1002,
  TRANS_ERR_RESIZE_INVALID_PARAM = 1003,
  TRANS_ERR_CROP_NO_INPUT = 1004,
  TRANS_ERR_CROP_INVALID_PARAM = 1005,
  TRANS_ERR_TRANSPOSE_NO_INPUT = 1006,
  TRANS_ERR_ROTATE_NO_INPUT = 1007,
  TRANS_ERR_ROTATE_INVALID_PARAM = 1008,
  TRANS_ERR_RAND_CROP_INVALID_PARAM = 1009,
  TRANS_ERR_FLIP_INVALID_PARAM = 1010,
};

/**
 * @brief base class to abstract the transformation on multiple type of data
 * sample,
 *        eg: image/video/audio
 *        processing steps:
 *        1, configure worker number, queue length
 *        2, configure operations needed to be applied on data samples
 *        3, launch this transformer
 *        4, feed data sample and fetch results
 */
class Transformer {
public:
  Transformer();
  virtual ~Transformer();

  /*
   * class method to create a transformer
   */
  static Transformer *create(const std::string &type);

  /*
   * class method to destroy a transformer
   */
  static void destroy(Transformer *t);

  /*
   * init this transformer
   */
  virtual int init(const kv_conf_t &conf) = 0;

  /*
   * add transformation operations to this transformer
   */
  virtual int add_op(const std::string &op_name, const kv_conf_t &conf) = 0;

  /*
   * launch this transformer to work
   */
  virtual int start() = 0;

  /*
   * stop this transformer
   */
  virtual int stop() = 0;

  /*
   * test whether this transformer has already stopped
   */
  virtual bool is_stopped() = 0;

  /*
   * put a new image processing task to this transformer
   */
  virtual int put(const transformer_input_data_t &input) = 0;

  /*
   * get a transformed image from this transformer
   */
  virtual int get(transformer_output_data_t *output) = 0;

  /*
   * put a new image processing task to this transformer
   */
  virtual int put(int id,
                  const char *image,
                  int image_len,
                  const char *label = "",
                  int label_len = 0) = 0;
};

};  // namespace vistool

#endif  // DATAREADER_CPP_INCLUDE_TRANSFORMER_H_