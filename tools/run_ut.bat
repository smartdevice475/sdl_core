@echo off
set UT_PATH=UT

mkdir %UT_PATH%

echo [01/18] application_manager_test
.\test_bin\application_manager_test\application_manager_test.exe > %UT_PATH%\application_manager_test.txt
echo [02/18] config_profile_test
.\test_bin\config_profile_test\config_profile_test.exe > %UT_PATH%\config_profile_test.txt
echo [03/18] connection_handler_test
.\test_bin\connection_handler_test\connection_handler_test.exe > %UT_PATH%\connection_handler_test.txt
echo [04/18] data_resumption_test
.\test_bin\data_resumption_test\data_resumption_test.exe > %UT_PATH%\data_resumption_test.txt
echo [05/18] formatters_test
.\test_bin\formatters_test\formatters_test.exe > %UT_PATH%\formatters_test.txt
echo [06/18] hmi_message_handler_test
.\test_bin\hmi_message_handler_test\hmi_message_handler_test.exe > %UT_PATH%\hmi_message_handler_test.txt
echo [07/18] media_manager_test
.\test_bin\media_manager_test\media_manager_test.exe > %UT_PATH%\media_manager_test.txt
echo [08/18] message_helper_test
.\test_bin\message_helper_test\message_helper_test.exe > %UT_PATH%\message_helper_test.txt
echo [09/18] policy_test
.\test_bin\policy_test\policy_test.exe > %UT_PATH%\policy_test.txt
echo [10/18] protocol_handler_test
.\test_bin\protocol_handler_test\protocol_handler_test.exe > %UT_PATH%\protocol_handler_test.txt
echo [11/18] resumption_test
.\test_bin\resumption_test\resumption_test.exe > %UT_PATH%\resumption_test.txt
echo [12/18] rpc_base_test
.\test_bin\rpc_base_test\rpc_base_test.exe > %UT_PATH%\rpc_base_test.txt
echo [13/18] security_manager_test
.\test_bin\security_manager_test\security_manager_test.exe > %UT_PATH%\security_manager_test.txt
echo [14/18] smart_object_test
.\test_bin\smart_object_test\smart_object_test.exe > %UT_PATH%\smart_object_test.txt
echo [15/18] state_controller_test
.\test_bin\state_controller_test\state_controller_test.exe > %UT_PATH%\state_controller_test.txt
echo [16/18] test_JSONCPP
.\test_bin\test_JSONCPP\test_JSONCPP.exe > %UT_PATH%\test_JSONCPP.txt
echo [17/18] transport_manager_test
.\test_bin\transport_manager_test\transport_manager_test.exe transport_manager_test.txt
echo [18/18] utils_test
.\test_bin\utils_test\utils_test.exe > %UT_PATH%\utils_test.txt
@echo on