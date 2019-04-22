
/*协议：

数据库
applicant里，sign = 0：未签到，
                  sign = 1：一面签到，
                  sign = 2：二面签到，
                  sign = 3：HR面签到

                  pass1、pass2、passHR：
                  0：未完成
                  1：通过
                  2：未通过
                  3: 通知
                  4：缺席
                  5:正在面试
                  6：完成面试
                  7：已表决

boss里，modifyEN->1：允许提交申请
                 0：截止提交申请

jobs里，applyNum、signNum、pass1、pass2、passHR表示已经顺利通过的记录

*/
