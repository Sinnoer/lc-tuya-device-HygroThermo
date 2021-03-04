/**********************************Copyright (c)**********************************
**                       鐗堟潈鎵�鏈� (C), 2015-2020, 娑傞甫绉戞妧
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  娑傞甫缁煎悎鍗忚寮�鍙戠粍
 * @version v2.5.6
 * @date    2020.12.16
 * @brief                
 *                       *******闈炲父閲嶈锛屼竴瀹氳鐪嬪摝锛侊紒锛�********
 *          1. 鐢ㄦ埛鍦ㄦ鏂囦欢涓疄鐜版暟鎹笅鍙�/涓婃姤鍔熻兘
 *          2. DP鐨処D/TYPE鍙婃暟鎹鐞嗗嚱鏁伴兘闇�瑕佺敤鎴锋寜鐓у疄闄呭畾涔夊疄鐜�
 *          3. 褰撳紑濮嬫煇浜涘畯瀹氫箟鍚庨渶瑕佺敤鎴峰疄鐜颁唬鐮佺殑鍑芥暟鍐呴儴鏈�#err鎻愮ず,瀹屾垚鍑芥暟鍚庤鍒犻櫎璇�#err
 */

/****************************** 鍏嶈矗澹版槑 锛侊紒锛� *******************************
鐢变簬MCU绫诲瀷鍜岀紪璇戠幆澧冨绉嶅鏍凤紝鎵�浠ユ浠ｇ爜浠呬緵鍙傝�冿紝鐢ㄦ埛璇疯嚜琛屾妸鎺ф渶缁堜唬鐮佽川閲忥紝
娑傞甫涓嶅MCU鍔熻兘缁撴灉璐熻矗銆�
******************************************************************************/

/******************************************************************************
                                绉绘椤荤煡:
1:MCU蹇呴』鍦╳hile涓洿鎺ヨ皟鐢╩cu_api.c鍐呯殑wifi_uart_service()鍑芥暟
2:绋嬪簭姝ｅ父鍒濆鍖栧畬鎴愬悗,寤鸿涓嶈繘琛屽叧涓插彛涓柇,濡傚繀椤诲叧涓柇,鍏充腑鏂椂闂村繀椤荤煭,鍏充腑鏂細寮曡捣涓插彛鏁版嵁鍖呬涪澶�
3:璇峰嬁鍦ㄤ腑鏂�/瀹氭椂鍣ㄤ腑鏂唴璋冪敤涓婃姤鍑芥暟
******************************************************************************/

#include "wifi.h"

#ifdef WEATHER_ENABLE
/**
 * @var    weather_choose
 * @brief  澶╂皵鏁版嵁鍙傛暟閫夋嫨鏁扮粍
 * @note   鐢ㄦ埛鍙互鑷畾涔夐渶瑕佺殑鍙傛暟锛屾敞閲婃垨鑰呭彇娑堟敞閲婂嵆鍙紝娉ㄦ剰鏇存敼
 */
const char *weather_choose[WEATHER_CHOOSE_CNT] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",
    "conditionNum",*/
};
#endif


/******************************************************************************
                              绗竴姝�:鍒濆鍖�
1:鍦ㄩ渶瑕佷娇鐢ㄥ埌wifi鐩稿叧鏂囦欢鐨勬枃浠朵腑include "wifi.h"
2:鍦∕CU鍒濆鍖栦腑璋冪敤mcu_api.c鏂囦欢涓殑wifi_protocol_init()鍑芥暟
3:灏哅CU涓插彛鍗曞瓧鑺傚彂閫佸嚱鏁板～鍏rotocol.c鏂囦欢涓璾art_transmit_output鍑芥暟鍐�,骞跺垹闄�#error
4:鍦∕CU涓插彛鎺ユ敹鍑芥暟涓皟鐢╩cu_api.c鏂囦欢鍐呯殑uart_receive_input鍑芥暟,骞跺皢鎺ユ敹鍒扮殑瀛楄妭浣滀负鍙傛暟浼犲叆
5:鍗曠墖鏈鸿繘鍏hile寰幆鍚庤皟鐢╩cu_api.c鏂囦欢鍐呯殑wifi_uart_service()鍑芥暟
******************************************************************************/

/******************************************************************************
                        1:dp鏁版嵁鐐瑰簭鍒楃被鍨嬪鐓ц〃
          **姝や负鑷姩鐢熸垚浠ｇ爜,濡傚湪寮�鍙戝钩鍙版湁鐩稿叧淇敼璇烽噸鏂颁笅杞組CU_SDK**
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},
  {DPID_HUMIDITY_VALUE, DP_TYPE_VALUE},
  {DPID_BATTERY_STATE, DP_TYPE_ENUM},
  {DPID_BATTERY_PERCENTAGE, DP_TYPE_VALUE},
  {DPID_TAMPER_ALARM, DP_TYPE_BOOL},
  {DPID_TEMP_SAMPLING, DP_TYPE_VALUE},
  {DPID_HUMIDITY_SAMPLING, DP_TYPE_VALUE},
  {DPID_CHARGE_STATE, DP_TYPE_BOOL},
  {DPID_TEMP_UNIT_CONVERT, DP_TYPE_ENUM},
  {DPID_MAXTEMP_SET, DP_TYPE_VALUE},
  {DPID_MINITEMP_SET, DP_TYPE_VALUE},
  {DPID_MAXHUM_SET, DP_TYPE_VALUE},
  {DPID_MINIHUM_SET, DP_TYPE_VALUE},
  {DPID_TEMP_ALARM, DP_TYPE_ENUM},
  {DPID_HUM_ALARM, DP_TYPE_ENUM},
  {DPID_BRIGHT_VALUE, DP_TYPE_VALUE},
  {DPID_TEMP_PERIODIC_REPORT, DP_TYPE_VALUE},
  {DPID_HUM_PERIODIC_REPORT, DP_TYPE_VALUE},
  {DPID_TEMP_SENSITIVITY, DP_TYPE_VALUE},
  {DPID_HUM_SENSITIVITY, DP_TYPE_VALUE},
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_TEMP_SET, DP_TYPE_VALUE},
  {DPID_HEARDBEAT, DP_TYPE_ENUM},
};



/******************************************************************************
                           2:涓插彛鍗曞瓧鑺傚彂閫佸嚱鏁�
璇峰皢MCU涓插彛鍙戦�佸嚱鏁板～鍏ヨ鍑芥暟鍐�,骞跺皢鎺ユ敹鍒扮殑鏁版嵁浣滀负鍙傛暟浼犲叆涓插彛鍙戦�佸嚱鏁�
******************************************************************************/
#include "include/Usart1.h"
/**
 * @brief  涓插彛鍙戦�佹暟鎹�
 * @param[in] {value} 涓插彛瑕佸彂閫佺殑1瀛楄妭鏁版嵁
 * @return Null
 */
void uart_transmit_output(unsigned char value)
{
//    #error "璇峰皢MCU涓插彛鍙戦�佸嚱鏁板～鍏ヨ鍑芥暟,骞跺垹闄よ琛�"
    rt_device_write(usart1_serial, 0, &value, 1);

    
/*
    //Example:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //涓插彛鍙戦�佸嚱鏁�
*/
}

/******************************************************************************
                           绗簩姝�:瀹炵幇鍏蜂綋鐢ㄦ埛鍑芥暟
1:APP涓嬪彂鏁版嵁澶勭悊
2:鏁版嵁涓婃姤澶勭悊
******************************************************************************/

/******************************************************************************
                            1:鎵�鏈夋暟鎹笂鎶ュ鐞�
褰撳墠鍑芥暟澶勭悊鍏ㄩ儴鏁版嵁涓婃姤(鍖呮嫭鍙笅鍙�/鍙笂鎶ュ拰鍙笂鎶�)
  闇�瑕佺敤鎴锋寜鐓у疄闄呮儏鍐靛疄鐜�:
  1:闇�瑕佸疄鐜板彲涓嬪彂/鍙笂鎶ユ暟鎹偣涓婃姤
  2:闇�瑕佸疄鐜板彧涓婃姤鏁版嵁鐐逛笂鎶�
姝ゅ嚱鏁颁负MCU鍐呴儴蹇呴』璋冪敤
鐢ㄦ埛涔熷彲璋冪敤姝ゅ嚱鏁板疄鐜板叏閮ㄦ暟鎹笂鎶�
******************************************************************************/

/**
 * @brief  绯荤粺鎵�鏈塪p鐐逛俊鎭笂浼�,瀹炵幇APP鍜宮uc鏁版嵁鍚屾
 * @param  Null
 * @return Null
 * @note   姝ゅ嚱鏁癝DK鍐呴儴闇�璋冪敤锛孧CU蹇呴』瀹炵幇璇ュ嚱鏁板唴鏁版嵁涓婃姤鍔熻兘锛屽寘鎷彧涓婃姤鍜屽彲涓婃姤鍙笅鍙戝瀷鏁版嵁
 */
void all_data_update(void)
{
//    #error "璇峰湪姝ゅ鐞嗗彲涓嬪彂鍙笂鎶ユ暟鎹強鍙笂鎶ユ暟鎹ず渚�,澶勭悊瀹屾垚鍚庡垹闄よ琛�"
//    /*
    //姝や唬鐮佷负骞冲彴鑷姩鐢熸垚锛岃鎸夌収瀹為檯鏁版嵁淇敼姣忎釜鍙笅鍙戝彲涓婃姤鍑芥暟鍜屽彧涓婃姤鍑芥暟
    mcu_dp_value_update(DPID_TEMP_CURRENT,send_thermo()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_HUMIDITY_VALUE,send_hygro()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_enum_update(DPID_BATTERY_STATE,send_battery_level()); //鏋氫妇鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,send_battery_percent()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_bool_update(DPID_TAMPER_ALARM,send_alarm_dismantle()); //BOOL鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_TEMP_SAMPLING,send_sampling_time_thermo()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_HUMIDITY_SAMPLING,send_sampling_time_hygro()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_bool_update(DPID_CHARGE_STATE,send_charge_state()); //BOOL鍨嬫暟鎹笂鎶�;
    mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,send_scale_switch()); //鏋氫妇鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_MAXTEMP_SET,send_limit_set_thermo_up()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_MINITEMP_SET,send_limit_set_thermo_down()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_MAXHUM_SET,send_limit_set_hygro_up()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_MINIHUM_SET,send_limit_set_hygro_down()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_enum_update(DPID_TEMP_ALARM,send_alarm_thermo()); //鏋氫妇鍨嬫暟鎹笂鎶�;
    mcu_dp_enum_update(DPID_HUM_ALARM,send_alarm_hygro()); //鏋氫妇鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_BRIGHT_VALUE,send_current_brightness()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_TEMP_PERIODIC_REPORT,send_periodic_reporting_thermo()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_HUM_PERIODIC_REPORT,send_periodic_reporting_hygro()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_TEMP_SENSITIVITY,send_sensitivity_thermo()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_HUM_SENSITIVITY,send_sensitivity_hygro()); //VALUE鍨嬫暟鎹笂鎶�;
    mcu_dp_bool_update(DPID_SWITCH,send_on_off()); //BOOL鍨嬫暟鎹笂鎶�;
    mcu_dp_value_update(DPID_TEMP_SET,send_seting_thermo()); //VALUE鍨嬫暟鎹笂鎶�;

//    */
}

/******************************************************************************
                                WARNING!!!    
                            2:鎵�鏈夋暟鎹笂鎶ュ鐞�
鑷姩鍖栦唬鐮佹ā鏉垮嚱鏁�,鍏蜂綋璇风敤鎴疯嚜琛屽疄鐜版暟鎹鐞�
******************************************************************************/
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_temp_sampling_handle
鍔熻兘鎻忚堪 : 閽堝DPID_TEMP_SAMPLING鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_temp_sampling_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long temp_sampling;
    
    temp_sampling = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_TEMP_SAMPLING,temp_sampling);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_humidity_sampling_handle
鍔熻兘鎻忚堪 : 閽堝DPID_HUMIDITY_SAMPLING鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_humidity_sampling_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long humidity_sampling;
    
    humidity_sampling = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_HUMIDITY_SAMPLING,humidity_sampling);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_temp_unit_convert_handle
鍔熻兘鎻忚堪 : 閽堝DPID_TEMP_UNIT_CONVERT鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_temp_unit_convert_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篍NUM
    unsigned char ret;
    unsigned char temp_unit_convert;
    
    temp_unit_convert = mcu_get_dp_download_enum(value,length);
    switch(temp_unit_convert) {
        case 0:emoto_end();
        break;
        case 1:ibmoto_begin(MOTO_DELAY);
        break;
        default:emoto_end();
        break;

    }
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT, temp_unit_convert);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_maxtemp_set_handle
鍔熻兘鎻忚堪 : 閽堝DPID_MAXTEMP_SET鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_maxtemp_set_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long maxtemp_set;
    
    maxtemp_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_MAXTEMP_SET,maxtemp_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_minitemp_set_handle
鍔熻兘鎻忚堪 : 閽堝DPID_MINITEMP_SET鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_minitemp_set_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long minitemp_set;
    
    minitemp_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_MINITEMP_SET,minitemp_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_maxhum_set_handle
鍔熻兘鎻忚堪 : 閽堝DPID_MAXHUM_SET鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_maxhum_set_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long maxhum_set;
    
    maxhum_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_MAXHUM_SET,maxhum_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_minihum_set_handle
鍔熻兘鎻忚堪 : 閽堝DPID_MINIHUM_SET鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_minihum_set_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long minihum_set;
    
    minihum_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_MINIHUM_SET,minihum_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_bright_value_handle
鍔熻兘鎻忚堪 : 閽堝DPID_BRIGHT_VALUE鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_bright_value_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long bright_value;
    
    bright_value = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_BRIGHT_VALUE,bright_value);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_temp_periodic_report_handle
鍔熻兘鎻忚堪 : 閽堝DPID_TEMP_PERIODIC_REPORT鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_temp_periodic_report_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long temp_periodic_report;
    
    temp_periodic_report = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_TEMP_PERIODIC_REPORT,temp_periodic_report);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_hum_periodic_report_handle
鍔熻兘鎻忚堪 : 閽堝DPID_HUM_PERIODIC_REPORT鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_hum_periodic_report_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long hum_periodic_report;
    
    hum_periodic_report = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_HUM_PERIODIC_REPORT,hum_periodic_report);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_temp_sensitivity_handle
鍔熻兘鎻忚堪 : 閽堝DPID_TEMP_SENSITIVITY鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_temp_sensitivity_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long temp_sensitivity;
    
    temp_sensitivity = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_TEMP_SENSITIVITY,temp_sensitivity);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_hum_sensitivity_handle
鍔熻兘鎻忚堪 : 閽堝DPID_HUM_SENSITIVITY鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_hum_sensitivity_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long hum_sensitivity;
    
    hum_sensitivity = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_HUM_SENSITIVITY,hum_sensitivity);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_switch_handle
鍔熻兘鎻忚堪 : 閽堝DPID_SWITCH鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓築OOL
    unsigned char ret;
    //0:鍏�/1:寮�
    unsigned char switch_1;
    
    switch_1 = mcu_get_dp_download_bool(value,length);
    if(switch_1 == 0) {
        emoto_end();//寮�鍏冲叧
    }else {
        ibmoto_begin(MOTO_DELAY);//寮�鍏冲紑
    }

    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_bool_update(DPID_SWITCH,switch_1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_temp_set_handle
鍔熻兘鎻忚堪 : 閽堝DPID_TEMP_SET鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戝彲涓婃姤绫诲瀷,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_temp_set_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篤ALUE
    unsigned char ret;
    unsigned long temp_set;
    
    temp_set = mcu_get_dp_download_value(value,length);
    /*
    //VALUE绫诲瀷鏁版嵁澶勭悊
    
    */
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_value_update(DPID_TEMP_SET,temp_set);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
鍑芥暟鍚嶇О : dp_download_heardbeat_handle
鍔熻兘鎻忚堪 : 閽堝DPID_HEARDBEAT鐨勫鐞嗗嚱鏁�
杈撳叆鍙傛暟 : value:鏁版嵁婧愭暟鎹�
        : length:鏁版嵁闀垮害
杩斿洖鍙傛暟 : 鎴愬姛杩斿洖:SUCCESS/澶辫触杩斿洖:ERROR
浣跨敤璇存槑 : 鍙笅鍙戠被鍨�,闇�瑕佸湪澶勭悊瀹屾暟鎹悗涓婃姤澶勭悊缁撴灉鑷砤pp
*****************************************************************************/
static unsigned char dp_download_heardbeat_handle(const unsigned char value[], unsigned short length)
{
    //绀轰緥:褰撳墠DP绫诲瀷涓篍NUM
    unsigned char ret;
    unsigned char heardbeat;
    
    heardbeat = mcu_get_dp_download_enum(value,length);
    switch(heardbeat) {
        case 0:emoto_end();
        break;
        case 1:ibmoto_begin(MOTO_DELAY);
        break;
        default:emoto_end();
        break;
    }
    
    //澶勭悊瀹孌P鏁版嵁鍚庡簲鏈夊弽棣�
    ret = mcu_dp_enum_update(DPID_HEARDBEAT, heardbeat);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




/******************************************************************************
                                WARNING!!!                     
姝ら儴鍒嗗嚱鏁扮敤鎴疯鍕夸慨鏀�!!
******************************************************************************/

/**
 * @brief  dp涓嬪彂澶勭悊鍑芥暟
 * @param[in] {dpid} dpid 搴忓彿
 * @param[in] {value} dp鏁版嵁缂撳啿鍖哄湴鍧�
 * @param[in] {length} dp鏁版嵁闀垮害
 * @return dp澶勭悊缁撴灉
 * -           0(ERROR): 澶辫触
 * -           1(SUCCESS): 鎴愬姛
 * @note   璇ュ嚱鏁扮敤鎴蜂笉鑳戒慨鏀�
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    /*********************************
    褰撳墠鍑芥暟澶勭悊鍙笅鍙�/鍙笂鎶ユ暟鎹皟鐢�
    鍏蜂綋鍑芥暟鍐呴渶瑕佸疄鐜颁笅鍙戞暟鎹鐞�
    瀹屾垚鐢ㄩ渶瑕佸皢澶勭悊缁撴灉鍙嶉鑷矨PP绔�,鍚﹀垯APP浼氳涓轰笅鍙戝け璐�
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_TEMP_SAMPLING:
            //娓╁害閲囨牱鏃堕棿璁剧疆澶勭悊鍑芥暟
            ret = dp_download_temp_sampling_handle(value,length);
        break;
        case DPID_HUMIDITY_SAMPLING:
            //婀垮害閲囨牱鏃堕棿璁剧疆澶勭悊鍑芥暟
            ret = dp_download_humidity_sampling_handle(value,length);
        break;
        case DPID_TEMP_UNIT_CONVERT:
            //娓╂爣鍒囨崲澶勭悊鍑芥暟
            ret = dp_download_temp_unit_convert_handle(value,length);
        break;
        case DPID_MAXTEMP_SET:
            //娓╁害涓婇檺璁剧疆澶勭悊鍑芥暟
            ret = dp_download_maxtemp_set_handle(value,length);
        break;
        case DPID_MINITEMP_SET:
            //娓╁害涓嬮檺璁剧疆澶勭悊鍑芥暟
            ret = dp_download_minitemp_set_handle(value,length);
        break;
        case DPID_MAXHUM_SET:
            //婀垮害涓婇檺璁剧疆澶勭悊鍑芥暟
            ret = dp_download_maxhum_set_handle(value,length);
        break;
        case DPID_MINIHUM_SET:
            //婀垮害涓嬮檺璁剧疆澶勭悊鍑芥暟
            ret = dp_download_minihum_set_handle(value,length);
        break;
        case DPID_BRIGHT_VALUE:
            //褰撳墠浜害鍊煎鐞嗗嚱鏁�
            ret = dp_download_bright_value_handle(value,length);
        break;
        case DPID_TEMP_PERIODIC_REPORT:
            //娓╁害鍛ㄦ湡涓婃姤澶勭悊鍑芥暟
            ret = dp_download_temp_periodic_report_handle(value,length);
        break;
        case DPID_HUM_PERIODIC_REPORT:
            //婀垮害鍛ㄦ湡涓婃姤澶勭悊鍑芥暟
            ret = dp_download_hum_periodic_report_handle(value,length);
        break;
        case DPID_TEMP_SENSITIVITY:
            //娓╁害鐏垫晱搴﹀鐞嗗嚱鏁�
            ret = dp_download_temp_sensitivity_handle(value,length);
        break;
        case DPID_HUM_SENSITIVITY:
            //婀垮害鐏垫晱搴﹀鐞嗗嚱鏁�
            ret = dp_download_hum_sensitivity_handle(value,length);
        break;
        case DPID_SWITCH:
            //寮�鍏冲鐞嗗嚱鏁�
            ret = dp_download_switch_handle(value,length);
        break;
        case DPID_TEMP_SET:
            //娓╁害璁剧疆澶勭悊鍑芥暟
            ret = dp_download_temp_set_handle(value,length);
        break;
        case DPID_HEARDBEAT:
            //蹇冭烦澶勭悊鍑芥暟
            ret = dp_download_heardbeat_handle(value,length);
        break;

        
        default:
        break;
    }
    return ret;
}

/**
 * @brief  鑾峰彇鎵�鏈塪p鍛戒护鎬诲拰
 * @param[in] Null
 * @return 涓嬪彂鍛戒护鎬诲拰
 * @note   璇ュ嚱鏁扮敤鎴蜂笉鑳戒慨鏀�
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/******************************************************************************
                                WARNING!!!                     
姝や唬鐮佷负SDK鍐呴儴璋冪敤,璇锋寜鐓у疄闄卍p鏁版嵁瀹炵幇鍑芥暟鍐呴儴鏁版嵁
******************************************************************************/

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  鍗囩骇鍖呭ぇ灏忛�夋嫨
 * @param[in] {package_sz} 鍗囩骇鍖呭ぇ灏�
 * @ref           0x00: 256byte (榛樿)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void upgrade_package_choose(unsigned char package_sz)
{
    #error "璇疯嚜琛屽疄鐜拌鑷瀹炵幇鍗囩骇鍖呭ぇ灏忛�夋嫨浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(UPDATE_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  MCU杩涘叆鍥轰欢鍗囩骇妯″紡
 * @param[in] {value} 鍥轰欢缂撳啿鍖�
 * @param[in] {position} 褰撳墠鏁版嵁鍖呭湪浜庡浐浠朵綅缃�
 * @param[in] {length} 褰撳墠鍥轰欢鍖呴暱搴�(鍥轰欢鍖呴暱搴︿负0鏃�,琛ㄧず鍥轰欢鍖呭彂閫佸畬鎴�)
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "璇疯嚜琛屽畬鎴怣CU鍥轰欢鍗囩骇浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(length == 0) {
        //鍥轰欢鏁版嵁鍙戦�佸畬鎴�
      
    }else {
        //鍥轰欢鏁版嵁澶勭悊
      
    }
    
    return SUCCESS;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  鑾峰彇鍒扮殑鏍兼灄鏃堕棿
 * @param[in] {time} 鑾峰彇鍒扮殑鏍兼灄鏃堕棿鏁版嵁
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void mcu_get_greentime(unsigned char time[])
{
    #error "璇疯嚜琛屽畬鎴愮浉鍏充唬鐮�,骞跺垹闄よ琛�"
    /*
    time[0] 涓烘槸鍚﹁幏鍙栨椂闂存垚鍔熸爣蹇楋紝涓� 0 琛ㄧず澶辫触锛屼负 1琛ㄧず鎴愬姛
    time[1] 涓哄勾浠斤紝0x00 琛ㄧず 2000 骞�
    time[2] 涓烘湀浠斤紝浠� 1 寮�濮嬪埌12 缁撴潫
    time[3] 涓烘棩鏈燂紝浠� 1 寮�濮嬪埌31 缁撴潫
    time[4] 涓烘椂閽燂紝浠� 0 寮�濮嬪埌23 缁撴潫
    time[5] 涓哄垎閽燂紝浠� 0 寮�濮嬪埌59 缁撴潫
    time[6] 涓虹閽燂紝浠� 0 寮�濮嬪埌59 缁撴潫
    */
    if(time[0] == 1) {
        //姝ｇ‘鎺ユ敹鍒皐ifi妯″潡杩斿洖鐨勬牸鏋楁暟鎹�
        
    }else {
        //鑾峰彇鏍兼灄鏃堕棿鍑洪敊,鏈夊彲鑳芥槸褰撳墠wifi妯″潡鏈仈缃�
    }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU鏍″鏈湴RTC鏃堕挓
 * @param[in] {time} 鑾峰彇鍒扮殑鏍兼灄鏃堕棿鏁版嵁
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void mcu_write_rtctime(unsigned char time[])
{
    #error "璇疯嚜琛屽畬鎴怰TC鏃堕挓鍐欏叆浠ｇ爜,骞跺垹闄よ琛�"
    /*
    Time[0] 涓烘槸鍚﹁幏鍙栨椂闂存垚鍔熸爣蹇楋紝涓� 0 琛ㄧず澶辫触锛屼负 1琛ㄧず鎴愬姛
    Time[1] 涓哄勾浠斤紝0x00 琛ㄧず 2000 骞�
    Time[2] 涓烘湀浠斤紝浠� 1 寮�濮嬪埌12 缁撴潫
    Time[3] 涓烘棩鏈燂紝浠� 1 寮�濮嬪埌31 缁撴潫
    Time[4] 涓烘椂閽燂紝浠� 0 寮�濮嬪埌23 缁撴潫
    Time[5] 涓哄垎閽燂紝浠� 0 寮�濮嬪埌59 缁撴潫
    Time[6] 涓虹閽燂紝浠� 0 寮�濮嬪埌59 缁撴潫
    Time[7] 涓烘槦鏈燂紝浠� 1 寮�濮嬪埌 7 缁撴潫锛�1浠ｈ〃鏄熸湡涓�
   */
    if(time[0] == 1) {
        //姝ｇ‘鎺ユ敹鍒皐ifi妯″潡杩斿洖鐨勬湰鍦版椂閽熸暟鎹�
     
    }else {
        //鑾峰彇鏈湴鏃堕挓鏁版嵁鍑洪敊,鏈夊彲鑳芥槸褰撳墠wifi妯″潡鏈仈缃�
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi鍔熻兘娴嬭瘯鍙嶉
 * @param[in] {result} wifi鍔熻兘娴嬭瘯缁撴灉
 * @ref       0: 澶辫触
 * @ref       1: 鎴愬姛
 * @param[in] {rssi} 娴嬭瘯鎴愬姛琛ㄧずwifi淇″彿寮哄害/娴嬭瘯澶辫触琛ㄧず閿欒绫诲瀷
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//    #error "璇疯嚜琛屽疄鐜皐ifi鍔熻兘娴嬭瘯鎴愬姛/澶辫触浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0) {
        //娴嬭瘯澶辫触
        if(rssi == 0x00) {
            //鏈壂鎻忓埌鍚嶇О涓簍uya_mdev_test璺敱鍣�,璇锋鏌�
        }else if(rssi == 0x01) {
            //妯″潡鏈巿鏉�
        }
    }else {
        //娴嬭瘯鎴愬姛
        //rssi涓轰俊鍙峰己搴�(0-100, 0淇″彿鏈�宸紝100淇″彿鏈�寮�)
    }
}
#endif

#ifdef WEATHER_ENABLE
/**
* @brief  mcu鎵撳紑澶╂皵鏈嶅姟
 * @param  Null
 * @return Null
 */
void mcu_open_weather(void)
{
    int i = 0;
    char buffer[13] = {0};
    unsigned char weather_len = 0;
    unsigned short send_len = 0;
    
    weather_len = sizeof(weather_choose) / sizeof(weather_choose[0]);
      
    for(i=0;i<weather_len;i++) {
        buffer[0] = sprintf(buffer+1,"w.%s",weather_choose[i]);
        send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    }
    
    #error "璇锋牴鎹彁绀猴紝鑷瀹屽杽鎵撳紑澶╂皵鏈嶅姟浠ｇ爜锛屽畬鎴愬悗璇峰垹闄よ琛�"
    /*
    //褰撹幏鍙栫殑鍙傛暟鏈夊拰鏃堕棿鏈夊叧鐨勫弬鏁版椂(濡�:鏃ュ嚭鏃ヨ惤)锛岄渶瑕佹惌閰峵.unix鎴栬�卼.local浣跨敤锛岄渶瑕佽幏鍙栫殑鍙傛暟鏁版嵁鏄寜鐓ф牸鏋楁椂闂磋繕鏄湰鍦版椂闂�
    buffer[0] = sprintf(buffer+1,"t.unix"); //鏍兼灄鏃堕棿   鎴栦娇鐢�  buffer[0] = sprintf(buffer+1,"t.local"); //鏈湴鏃堕棿
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    */
    
    buffer[0] = sprintf(buffer+1,"w.date.%d",WEATHER_FORECAST_DAYS_NUM);
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    
    wifi_uart_write_frame(WEATHER_OPEN_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  鎵撳紑澶╂皵鍔熻兘杩斿洖鐢ㄦ埛鑷鐞嗗嚱鏁�
 * @param[in] {res} 鎵撳紑澶╂皵鍔熻兘杩斿洖缁撴灉
 * @ref       0: 澶辫触
 * @ref       1: 鎴愬姛
 * @param[in] {err} 閿欒鐮�
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void weather_open_return_handle(unsigned char res, unsigned char err)
{
    #error "璇疯嚜琛屽畬鎴愭墦寮�澶╂皵鍔熻兘杩斿洖鏁版嵁澶勭悊浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    unsigned char err_num = 0;
    
    if(res == 1) {
        //鎵撳紑澶╂皵杩斿洖鎴愬姛
    }else if(res == 0) {
        //鎵撳紑澶╂皵杩斿洖澶辫触
        //鑾峰彇閿欒鐮�
        err_num = err; 
    }
}

/**
 * @brief  澶╂皵鏁版嵁鐢ㄦ埛鑷鐞嗗嚱鏁�
 * @param[in] {name} 鍙傛暟鍚�
 * @param[in] {type} 鍙傛暟绫诲瀷
 * @ref       0: int 鍨�
 * @ref       1: string 鍨�
 * @param[in] {data} 鍙傛暟鍊肩殑鍦板潃
 * @param[in] {day} 鍝竴澶╃殑澶╂皵  0:琛ㄧず褰撳ぉ 鍙栧�艰寖鍥�: 0~6
 * @ref       0: 浠婂ぉ
 * @ref       1: 鏄庡ぉ
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void weather_data_user_handle(char *name, unsigned char type, const unsigned char *data, char day)
{
    #error "杩欓噷浠呯粰鍑虹ず渚嬶紝璇疯嚜琛屽畬鍠勫ぉ姘旀暟鎹鐞嗕唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    int value_int;
    char value_string[50];//鐢变簬鏈夌殑鍙傛暟鍐呭杈冨锛岃繖閲岄粯璁や负50銆傛偍鍙互鏍规嵁瀹氫箟鐨勫弬鏁帮紝鍙互閫傚綋鍑忓皯璇ュ��
    
    my_memset(value_string, '\0', 50);
    
    //棣栧厛鑾峰彇鏁版嵁绫诲瀷
    if(type == 0) { //鍙傛暟鏄疘NT鍨�
        value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }else if(type == 1) {
        my_strcpy(value_string, data);
    }
    
    //娉ㄦ剰瑕佹牴鎹墍閫夊弬鏁扮被鍨嬫潵鑾峰緱鍙傛暟鍊硷紒锛侊紒
    if(my_strcmp(name, "temp") == 0) {
        printf("day:%d temp value is:%d\r\n", day, value_int);          //int 鍨�
    }else if(my_strcmp(name, "humidity") == 0) {
        printf("day:%d humidity value is:%d\r\n", day, value_int);      //int 鍨�
    }else if(my_strcmp(name, "pm25") == 0) {
        printf("day:%d pm25 value is:%d\r\n", day, value_int);          //int 鍨�
    }else if(my_strcmp(name, "condition") == 0) {
        printf("day:%d condition value is:%s\r\n", day, value_string);  //string 鍨�
    }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  鐘舵�佸悓姝ヤ笂鎶ョ粨鏋�
 * @param[in] {result} 缁撴灉
 * @ref       0: 澶辫触
 * @ref       1: 鎴愬姛
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void get_upload_syn_result(unsigned char result)
{
    #error "璇疯嚜琛屽畬鎴愮姸鎬佸悓姝ヤ笂鎶ョ粨鏋滀唬鐮�,骞跺垹闄よ琛�"
      
    if(result == 0) {
        //鍚屾涓婃姤鍑洪敊
    }else {
        //鍚屾涓婃姤鎴愬姛
    }
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  鑾峰彇 WIFI 鐘舵�佺粨鏋�
 * @param[in] {result} 鎸囩ず WIFI 宸ヤ綔鐘舵��
 * @ref       0x00: wifi鐘舵�� 1 smartconfig 閰嶇疆鐘舵��
 * @ref       0x01: wifi鐘舵�� 2 AP 閰嶇疆鐘舵��
 * @ref       0x02: wifi鐘舵�� 3 WIFI 宸查厤缃絾鏈繛涓婅矾鐢卞櫒
 * @ref       0x03: wifi鐘舵�� 4 WIFI 宸查厤缃笖杩炰笂璺敱鍣�
 * @ref       0x04: wifi鐘舵�� 5 宸茶繛涓婅矾鐢卞櫒涓旇繛鎺ュ埌浜戠
 * @ref       0x05: wifi鐘舵�� 6 WIFI 璁惧澶勪簬浣庡姛鑰楁ā寮�
 * @ref       0x06: wifi鐘舵�� 7 WIFI 璁惧澶勪簬smartconfig&AP閰嶇疆鐘舵��
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void get_wifi_status(unsigned char result)
{
  #error "璇疯嚜琛屽畬鎴愯幏鍙� WIFI 鐘舵�佺粨鏋滀唬鐮�,骞跺垹闄よ琛�"
 
    switch(result) {
        case 0:
            //wifi宸ヤ綔鐘舵��1
        break;
    
        case 1:
            //wifi宸ヤ綔鐘舵��2
        break;
        
        case 2:
            //wifi宸ヤ綔鐘舵��3
        break;
        
        case 3:
            //wifi宸ヤ綔鐘舵��4
        break;
        
        case 4:
            //wifi宸ヤ綔鐘舵��5
        break;
        
        case 5:
            //wifi宸ヤ綔鐘舵��6
        break;
      
        case 6:
            //wifi宸ヤ綔鐘舵��7
        break;
        
        default:break;
    }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  娴佹湇鍔″彂閫佺粨鏋�
 * @param[in] {result} 缁撴灉
 * @ref       0x00: 鎴愬姛
 * @ref       0x01: 娴佹湇鍔″姛鑳芥湭寮�鍚�
 * @ref       0x02: 娴佹湇鍔″櫒鏈繛鎺ユ垚鍔�
 * @ref       0x03: 鏁版嵁鎺ㄩ�佽秴鏃�
 * @ref       0x04: 浼犺緭鐨勬暟鎹暱搴﹂敊璇�
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void stream_trans_send_result(unsigned char result)
{
    #error "杩欓噷浠呯粰鍑虹ず渚嬶紝璇疯嚜琛屽畬鍠勬祦鏈嶅姟鍙戦�佺粨鏋滃鐞嗕唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    switch(result) {
        case 0x00:
            //鎴愬姛
        break;
        
        case 0x01:
            //娴佹湇鍔″姛鑳芥湭寮�鍚�
        break;
        
        case 0x02:
            //娴佹湇鍔″櫒鏈繛鎺ユ垚鍔�
        break;
        
        case 0x03:
            //鏁版嵁鎺ㄩ�佽秴鏃�
        break;
        
        case 0x04:
            //浼犺緭鐨勬暟鎹暱搴﹂敊璇�
        break;
        
        default:break;
    }
}

/**
 * @brief  澶氬湴鍥炬祦鏈嶅姟鍙戦�佺粨鏋�
 * @param[in] {result} 缁撴灉
 * @ref       0x00: 鎴愬姛
 * @ref       0x01: 澶辫触
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void maps_stream_trans_send_result(unsigned char result)
{
    #error "杩欓噷浠呯粰鍑虹ず渚嬶紝璇疯嚜琛屽畬鍠勫鍦板浘娴佹湇鍔″彂閫佺粨鏋滃鐞嗕唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    switch(result) {
        case 0x00:
            //鎴愬姛
        break;
        
        case 0x01:
            //澶辫触
        break;
        
        default:break;
    }
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  璺敱淇℃伅鎺ユ敹缁撴灉閫氱煡
 * @param[in] {result} 妯″潡鏄惁鎴愬姛鎺ユ敹鍒版纭殑璺敱淇℃伅
 * @ref       0x00: 澶辫触
 * @ref       0x01: 鎴愬姛
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void wifi_connect_test_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜皐ifi鍔熻兘娴嬭瘯鎴愬姛/澶辫触浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0) {
        //璺敱淇℃伅鎺ユ敹澶辫触锛岃妫�鏌ュ彂鍑虹殑璺敱淇℃伅鍖呮槸鍚︽槸瀹屾暣鐨凧SON鏁版嵁鍖�
    }else {
        //璺敱淇℃伅鎺ユ敹鎴愬姛锛屼骇娴嬬粨鏋滆娉ㄦ剰WIFI_STATE_CMD鎸囦护鐨剋ifi宸ヤ綔鐘舵��
    }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  鑾峰彇妯″潡mac缁撴灉
 * @param[in] {mac} 妯″潡 MAC 鏁版嵁
 * @ref       mac[0]: 涓烘槸鍚﹁幏鍙杕ac鎴愬姛鏍囧織锛�0x00 琛ㄧず鎴愬姛锛�0x01 琛ㄧず澶辫触
 * @ref       mac[1]~mac[6]: 褰撹幏鍙� MAC鍦板潃鏍囧織浣嶅鏋渕ac[0]涓烘垚鍔燂紝鍒欒〃绀烘ā鍧楁湁鏁堢殑MAC鍦板潃
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void mcu_get_mac(unsigned char mac[])
{
    #error "璇疯嚜琛屽畬鎴恗ac鑾峰彇浠ｇ爜,骞跺垹闄よ琛�"
    /*
    mac[0]涓烘槸鍚﹁幏鍙杕ac鎴愬姛鏍囧織锛�0x00 琛ㄧず鎴愬姛锛屼负0x01琛ㄧず澶辫触
    mac[1]~mac[6]:褰撹幏鍙� MAC鍦板潃鏍囧織浣嶅鏋渕ac[0]涓烘垚鍔燂紝鍒欒〃绀烘ā鍧楁湁鏁堢殑MAC鍦板潃
   */
   
    if(mac[0] == 1) {
        //鑾峰彇mac鍑洪敊
    }else {
        //姝ｇ‘鎺ユ敹鍒皐ifi妯″潡杩斿洖鐨刴ac鍦板潃
    }
}
#endif

#ifdef GET_IR_STATUS_ENABLE
/**
 * @brief  鑾峰彇绾㈠鐘舵�佺粨鏋�
 * @param[in] {result} 鎸囩ず绾㈠鐘舵��
 * @ref       0x00: 绾㈠鐘舵�� 1 姝ｅ湪鍙戦�佺孩澶栫爜
 * @ref       0x01: 绾㈠鐘舵�� 2 鍙戦�佺孩澶栫爜缁撴潫
 * @ref       0x02: 绾㈠鐘舵�� 3 绾㈠瀛︿範寮�濮�
 * @ref       0x03: 绾㈠鐘舵�� 4 绾㈠瀛︿範缁撴潫
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void get_ir_status(unsigned char result)
{
    #error "璇疯嚜琛屽畬鎴愮孩澶栫姸鎬佷唬鐮�,骞跺垹闄よ琛�"
    switch(result) {
        case 0:
            //绾㈠鐘舵�� 1
        break;
      
        case 1:
            //绾㈠鐘舵�� 2
        break;
          
        case 2:
            //绾㈠鐘舵�� 3
        break;
          
        case 3:
            //绾㈠鐘舵�� 4
        break;
          
        default:break;
    }
    
    wifi_uart_write_frame(GET_IR_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  绾㈠杩涘叆鏀跺彂浜ф祴缁撴灉閫氱煡
 * @param[in] {result} 妯″潡鏄惁鎴愬姛鎺ユ敹鍒版纭殑淇℃伅
 * @ref       0x00: 澶辫触
 * @ref       0x01: 鎴愬姛
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void ir_tx_rx_test_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜扮孩澶栬繘鍏ユ敹鍙戜骇娴嬪姛鑳芥祴璇曟垚鍔�/澶辫触浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0) {
        //绾㈠杩涘叆鏀跺彂浜ф祴鎴愬姛
    }else {
        //绾㈠杩涘叆鏀跺彂浜ф祴澶辫触锛岃妫�鏌ュ彂鍑虹殑鏁版嵁鍖�
    }
}
#endif

#ifdef FILE_DOWNLOAD_ENABLE
/**
 * @brief  鏂囦欢涓嬭浇鍖呭ぇ灏忛�夋嫨
 * @param[in] {package_sz} 鏂囦欢涓嬭浇鍖呭ぇ灏�
 * @ref       0x00: 256 byte (榛樿)
 * @ref       0x01: 512 byte
 * @ref       0x02: 1024 byte
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void file_download_package_choose(unsigned char package_sz)
{
    #error "璇疯嚜琛屽疄鐜拌鑷瀹炵幇鏂囦欢涓嬭浇鍖呭ぇ灏忛�夋嫨浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(FILE_DOWNLOAD_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  鏂囦欢鍖呬笅杞芥ā寮�
 * @param[in] {value} 鏁版嵁缂撳啿鍖�
 * @param[in] {position} 褰撳墠鏁版嵁鍖呭湪浜庢枃浠朵綅缃�
 * @param[in] {length} 褰撳墠鏂囦欢鍖呴暱搴�(闀垮害涓�0鏃�,琛ㄧず鏂囦欢鍖呭彂閫佸畬鎴�)
 * @return 鏁版嵁澶勭悊缁撴灉
 * -           0(ERROR): 澶辫触
 * -           1(SUCCESS): 鎴愬姛
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
unsigned char file_download_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "璇疯嚜琛屽畬鎴愭枃浠跺寘涓嬭浇浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(length == 0) {
        //鏂囦欢鍖呮暟鎹彂閫佸畬鎴�
        
    }else {
        //鏂囦欢鍖呮暟鎹鐞�
      
    }
    
    return SUCCESS;
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  鎵撳紑妯″潡鏃堕棿鏈嶅姟閫氱煡缁撴灉
 * @param[in] {value} 鏁版嵁缂撳啿鍖�
 * @param[in] {length} 鏁版嵁闀垮害
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void open_module_time_serve_result(const unsigned char value[], unsigned short length)
{
    #error "璇疯嚜琛屽疄鐜版ā鍧楁椂闂存湇鍔￠�氱煡缁撴灉浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    unsigned char sub_cmd = value[0];
    
    switch(sub_cmd) {
        case 0x01: { //瀛愬懡浠�  鎵撳紑妯″潡鏃堕棿鏈嶅姟閫氱煡
            if(0x02 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            if(value[1] == 0) {
                //鏈嶅姟寮�鍚垚鍔�
            }else {
                //鏈嶅姟寮�鍚け璐�
            }
        }
        break;
        
        case 0x02: {  //瀛愬懡浠�  妯″潡鏃堕棿鏈嶅姟閫氱煡
            if(0x09 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            unsigned char time_type = value[1]; //0x00:鏍兼灄鏃堕棿  0x01:鏈湴鏃堕棿
            unsigned char time_data[7];
            
            my_memcpy(time_data, value + 2, length - 2);
            /*
            Data[0]涓哄勾浠�, 0x00琛ㄧず2000骞�
            Data[1]涓烘湀浠斤紝浠�1寮�濮嬪埌12缁撴潫
            Data[2]涓烘棩鏈燂紝浠�1寮�濮嬪埌31缁撴潫
            Data[3]涓烘椂閽燂紝浠�0寮�濮嬪埌23缁撴潫
            Data[4]涓哄垎閽燂紝浠�0寮�濮嬪埌59缁撴潫
            Data[5]涓虹閽燂紝浠�0寮�濮嬪埌15缁撴潫
            Data[6]涓烘槦鏈燂紝浠�1寮�濮嬪埌7缁撴潫锛�1浠ｈ〃鏄熸湡涓�
            */
            
            //鍦ㄦ澶勬坊鍔犳椂闂存暟鎹鐞嗕唬鐮侊紝time_type涓烘椂闂寸被鍨�
            
            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len,sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;
        
        case 0x03: {  //瀛愬懡浠�  涓诲姩璇锋眰澶╂皵鏈嶅姟鏁版嵁
            if(0x02 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            if(value[1] == 0) {
                //鎴愬姛
            }else {
                //澶辫触
            }
        }
        break;
        
        case 0x04: {  //瀛愬懡浠�  鎵撳紑妯″潡閲嶇疆鐘舵�侀�氱煡
            if(0x02 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            if(value[1] == 0) {
                //鎴愬姛
            }else {
                //澶辫触
            }
        }
        break;
        
        case 0x05: {  //瀛愬懡浠�  妯″潡閲嶇疆鐘舵�侀�氱煡
            if(0x02 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            switch(value[1]) {
                case 0x00:
                    //妯″潡鏈湴閲嶇疆
                    
                break;
                case 0x01:
                    //APP杩滅▼閲嶇疆
                    
                break;
                case 0x02:
                    //APP鎭㈠鍑哄巶閲嶇疆
                    
                break;
                default:break;
            }
            
            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;
        
        default:break;
    }
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  钃濈墮鍔熻兘鎬ф祴璇曠粨鏋�
 * @param[in] {value} 鏁版嵁缂撳啿鍖�
 * @param[in] {length} 鏁版嵁闀垮害
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void BLE_test_result(const unsigned char value[], unsigned short length)
{
    #error "璇疯嚜琛屽疄鐜拌摑鐗欏姛鑳芥�ф祴璇曠粨鏋滀唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    unsigned char sub_cmd = value[0];
    
    if(0x03 != length) {
        //鏁版嵁闀垮害閿欒
        return;
    }
    
    if(0x01 != sub_cmd) {
        //瀛愬懡浠ら敊璇�
        return;
    }
    
    unsigned char result = value[1];
    unsigned char rssi = value[2];
        
    if(result == 0) {
        //娴嬭瘯澶辫触
        if(rssi == 0x00) {
            //鏈壂鎻忓埌鍚嶇О涓� ty_mdev钃濈墮淇℃爣,璇锋鏌�
        }else if(rssi == 0x01) {
            //妯″潡鏈巿鏉�
        }
    }else if(result == 0x01) {
        //娴嬭瘯鎴愬姛
        //rssi涓轰俊鍙峰己搴�(0-100, 0淇″彿鏈�宸紝100淇″彿鏈�寮�)
    }
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  鑾峰彇璇煶鐘舵�佺爜缁撴灉
 * @param[in] {result} 璇煶鐘舵�佺爜
 * @ref       0x00: 绌洪棽
 * @ref       0x01: mic闈欓煶鐘舵��
 * @ref       0x02: 鍞ら啋
 * @ref       0x03: 姝ｅ湪褰曢煶
 * @ref       0x04: 姝ｅ湪璇嗗埆
 * @ref       0x05: 璇嗗埆鎴愬姛
 * @ref       0x06: 璇嗗埆澶辫触
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void get_voice_state_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜拌幏鍙栬闊崇姸鎬佺爜缁撴灉澶勭悊浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    switch(result) {
        case 0:
            //绌洪棽
        break;
    
        case 1:
            //mic闈欓煶鐘舵��
        break;
        
        case 2:
            //鍞ら啋
        break;
        
        case 3:
            //姝ｅ湪褰曢煶
        break;
        
        case 4:
            //姝ｅ湪璇嗗埆
        break;
    
        case 5:
            //璇嗗埆鎴愬姛
        break;
        
        case 6:
            //璇嗗埆澶辫触
        break;
        
      default:break;
    }
}

/**
 * @brief  MIC闈欓煶璁剧疆缁撴灉
 * @param[in] {result} 璇煶鐘舵�佺爜
 * @ref       0x00: mic 寮�鍚�
 * @ref       0x01: mic 闈欓煶
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void set_voice_MIC_silence_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜癕IC闈欓煶璁剧疆澶勭悊浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0) {
        //mic 寮�鍚�
    }else {
        //mic 闈欓煶
    }
}

/**
 * @brief  speaker闊抽噺璁剧疆缁撴灉
 * @param[in] {result} 闊抽噺鍊�
 * @ref       0~10: 闊抽噺鑼冨洿
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void set_speaker_voice_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜皊peaker闊抽噺璁剧疆缁撴灉澶勭悊浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    
}

/**
 * @brief  闊抽浜ф祴缁撴灉
 * @param[in] {result} 闊抽浜ф祴鐘舵��
 * @ref       0x00: 鍏抽棴闊抽浜ф祴
 * @ref       0x01: mic1闊抽鐜矾娴嬭瘯
 * @ref       0x02: mic2闊抽鐜矾娴嬭瘯
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void voice_test_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜伴煶棰戜骇娴嬬粨鏋滃鐞嗕唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0x00) {
        //鍏抽棴闊抽浜ф祴
    }else if(result == 0x01) {
        //mic1闊抽鐜矾娴嬭瘯
    }else if(result == 0x02) {
        //mic2闊抽鐜矾娴嬭瘯
    }
}

/**
 * @brief  鍞ら啋浜ф祴缁撴灉
 * @param[in] {result} 鍞ら啋杩斿洖鍊�
 * @ref       0x00: 鍞ら啋鎴愬姛
 * @ref       0x01: 鍞ら啋澶辫触(10s瓒呮椂澶辫触)
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void voice_awaken_test_result(unsigned char result)
{
    #error "璇疯嚜琛屽疄鐜板敜閱掍骇娴嬬粨鏋滃鐞嗕唬鐮�,瀹屾垚鍚庤鍒犻櫎璇ヨ"
    if(result == 0x00) {
        //鍞ら啋鎴愬姛
    }else if(result == 0x01) {
        //鍞ら啋澶辫触
    }
}

/**
 * @brief  璇煶妯＄粍鎵╁睍鍔熻兘
 * @param[in] {value} 鏁版嵁缂撳啿鍖�
 * @param[in] {length} 鏁版嵁闀垮害
 * @return Null
 * @note   MCU闇�瑕佽嚜琛屽疄鐜拌鍔熻兘
 */
void voice_module_extend_fun(const unsigned char value[], unsigned short length)
{
    unsigned char sub_cmd = value[0];
    unsigned char play;
    unsigned char bt_play;
    unsigned short send_len = 0;
  
    switch(sub_cmd) {
        case 0x00: { //瀛愬懡浠�  MCU鍔熻兘璁剧疆
            if(0x02 != length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            if(value[1] == 0) {
                //鎴愬姛
            }else {
                //澶辫触
            }
        }
        break;
        
        case 0x01: {  //瀛愬懡浠�  鐘舵�侀�氱煡
            if(0x02 > length) {
                //鏁版嵁闀垮害閿欒
                return;
            }
            
            unsigned char play = 0xff;
            unsigned char bt_play = 0xff;
            
            const char *str_buff = (const char *)&value[1];
            const char *str_result = NULL;
            
            str_result = strstr(str_buff,"play") + my_strlen("play") + 2;
            if(NULL == str_result) {
                //鏁版嵁閿欒
                goto ERR_EXTI;
            }
            
            if(0 == memcmp(str_result, "true", my_strlen("true"))) {
                play = 1;
            }else if(0 == memcmp(str_result, "false", my_strlen("false"))) {
                play = 0;
            }else {
                //鏁版嵁閿欒
                goto ERR_EXTI;
            }
            
            str_result = strstr(str_buff,"bt_play") + my_strlen("bt_play") + 2;
            if(NULL == str_result) {
                //鏁版嵁閿欒
                goto ERR_EXTI;
            }
            
            if(0 == memcmp(str_result, "true", my_strlen("true"))) {
                bt_play = 1;
            }else if(0 == memcmp(str_result, "false", my_strlen("false"))) {
                bt_play = 0;
            }else {
                //鏁版嵁閿欒
                goto ERR_EXTI;
            }
            
            #error "璇疯嚜琛屽疄鐜拌闊虫ā缁勭姸鎬侀�氱煡澶勭悊浠ｇ爜,瀹屾垚鍚庤鍒犻櫎璇ヨ"
            //MCU璁剧疆鏆備粎鏀寔鈥濇挱鏀�/鏆傚仠鈥� 鈥濊摑鐗欏紑鍏斥��
            //play    鎾斁/鏆傚仠鍔熻兘  1(鎾斁) / 0(鏆傚仠)
            //bt_play 钃濈墮寮�鍏冲姛鑳�   1(寮�)   / 0(鍏�)
            
            
            
            send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            send_len = set_wifi_uart_byte(send_len, 0x00);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;

        default:break;
    }
    
    return;

ERR_EXTI:
    send_len = 0;
    send_len = set_wifi_uart_byte(send_len, sub_cmd);
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    return;
}
#endif




