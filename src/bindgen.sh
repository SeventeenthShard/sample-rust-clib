#!/bin/bash

bindgen MyLib.h -o bindings.rs --allowlist-function 'MyLib.*' --rustified-enum 'MyLib.*type.*'

