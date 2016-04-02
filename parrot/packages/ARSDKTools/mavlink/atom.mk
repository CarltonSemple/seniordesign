LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_DESCRIPTION := MAVLink Protocole Library
LOCAL_CATEGORY_PATH := dragon/tools
LOCAL_MODULE := libmavlink

LOCAL_INSTALL_HEADERS := \
	mavlink/checksum.h:usr/include/mavlink/ \
	mavlink/common/common.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_attitude.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_attitude_quaternion.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_auth_key.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_battery_status.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_change_operator_control_ack.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_change_operator_control.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_command_ack.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_command_long.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_data_stream.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_debug.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_debug_vect.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_file_transfer_dir_list.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_file_transfer_res.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_file_transfer_start.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_global_position_int.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_global_position_setpoint_int.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_global_vision_position_estimate.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_gps_global_origin.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_gps_raw_int.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_gps_status.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_heartbeat.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_highres_imu.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_hil_controls.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_hil_rc_inputs_raw.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_hil_state.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_local_position_ned.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_local_position_ned_system_global_offset.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_local_position_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_manual_control.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_manual_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_memory_vect.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_ack.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_clear_all.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_count.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_current.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_item.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_item_reached.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_request.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_request_list.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_request_partial_list.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_set_current.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_mission_write_partial_list.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_named_value_float.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_named_value_int.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_nav_controller_output.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_optical_flow.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_param_request_list.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_param_request_read.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_param_set.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_param_value.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_ping.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_raw_imu.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_raw_pressure.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_rc_channels_override.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_rc_channels_raw.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_rc_channels_scaled.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_request_data_stream.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_roll_pitch_yaw_rates_thrust_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_roll_pitch_yaw_speed_thrust_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_roll_pitch_yaw_thrust_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_safety_allowed_area.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_safety_set_allowed_area.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_scaled_imu.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_scaled_pressure.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_servo_output_raw.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_global_position_setpoint_int.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_gps_global_origin.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_local_position_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_mode.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_setpoint_6dof.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_setpoint_8dof.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_quad_motors_setpoint.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_quad_swarm_led_roll_pitch_yaw_thrust.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_quad_swarm_roll_pitch_yaw_thrust.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_roll_pitch_yaw_speed_thrust.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_set_roll_pitch_yaw_thrust.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_state_correction.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_statustext.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_sys_status.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_system_time.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_vfr_hud.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_vicon_position_estimate.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_vision_position_estimate.h:usr/include/mavlink/common/ \
	mavlink/common/mavlink_msg_vision_speed_estimate.h:usr/include/mavlink/common/ \
	mavlink/common/testsuite.h:usr/include/mavlink/common/ \
	mavlink/common/version.h:usr/include/mavlink/common/ \
	mavlink/mavlink_helpers.h:usr/include/mavlink/ \
	mavlink/mavlink_protobuf_manager.hpp:usr/include/mavlink/ \
	mavlink/mavlink_types.h:usr/include/mavlink/ \
	mavlink/protocol.h:usr/include/mavlink/

include $(BUILD_CUSTOM)
