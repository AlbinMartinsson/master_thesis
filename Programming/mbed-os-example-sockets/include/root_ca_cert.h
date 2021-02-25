/* Sockets Example
 * Copyright (c) 2020 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
static const char root_ca_cert[] = \
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
    "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
    "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
    "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
    "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
    "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
    "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
    "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
    "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
    "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
    "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
    "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
    "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
    "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
    "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
    "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
    "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
    "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
    "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
    "-----END CERTIFICATE-----";
*/

static const char root_ca_cert[] = \
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDeDCCAmCgAwIBAgIEYCpF7jANBgkqhkiG9w0BAQsFADAoMSYwJAYDVQQDDB10\n"
    "ZXN0Y2xvdWQyLmFpdGlhLmFycm93aGVhZC5ldTAeFw0yMTAyMTUwOTU5MTBaFw0z\n"
    "MTAyMTUwOTU5MTBaMDExLzAtBgNVBAMMJnByb3ZpZGVyLnRlc3RjbG91ZDIuYWl0\n"
    "aWEuYXJyb3doZWFkLmV1MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
    "v+98GJyrboGq696lIpRYHp2K1Tv7hc41w26T4lw1lpk/pmfnxOFeuc/trFn28n7F\n"
    "t0o10pKBSZj02fSsic/3AlrPM+zpuRND2NVjkYblXqg8dJXjLb6otEkItQ9e7n2h\n"
    "kQE8Ul/qNv/7WUZLyWDtntrBU0wUbNqCVbXrdbjEZDkpNevR3wZJGOgifAdzXpj4\n"
    "BTqQHeIC17sIKqYZwumCE9tF6clvWeFS9KoyOT8vykMsjMl7qaMA/FqCIRduZ1iZ\n"
    "IEyPp1plGksnPso86OwensHyL31h/LKi9pgE5wyI6lYxUJWDW9Tw7g+mwutRuain\n"
    "vYFxPF65U0w6unt1+YclFQIDAQABo4GgMIGdMDgGA1UdEQQxMC+CCWxvY2FsaG9z\n"
    "dIcEfwAAAYcEwKgAaIcEwKgAaYcEwKgAaocEwKgAa4cEwKgAbDBCBgNVHSMEOzA5\n"
    "gBQEME0II5M93mY9Flqn0cleI6aXT6EbpBkwFzEVMBMGA1UEAwwMYXJyb3doZWFk\n"
    "LmV1ggRc2WIZMB0GA1UdDgQWBBRiMHfMV+oxYAGf/WLz3NYP2vn8ODANBgkqhkiG\n"
    "9w0BAQsFAAOCAQEAaHrwAGPl1H8ENoBCmhvgxjSVJaR2pjZZXd2RSQwOiavUtvvF\n"
    "EWMVDGpMUU7J7y95cAsmlxTOVICSDcJg7UmeOxouExMP4uS2SMFoj/FS/K4Nhpti\n"
    "pbfrDZiNNlVMW1jIHF3oHKnveAdedCAuWHfPoRLOYaNTfjpne2RsU0TbO6Ur6DQP\n"
    "D7dW/wDkqS4CGAyjFc60nwWQ43/Zkf3hNt2B3n71wtyT+XXL0qnwWM3VhsCinXbo\n"
    "GAsrlKlUKk4qmThhwEsW1+Sre982JFU0hGcF2HDcE059p0F7Uy//Pn91xaQRXvVI\n"
    "yFORtCgHCXMKXePLCX08Gx+QUcZUJUE8xB11MQ==\n"
    "-----END CERTIFICATE-----";




