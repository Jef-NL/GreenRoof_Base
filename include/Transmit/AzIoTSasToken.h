/**
 * @file AzIoTSasToken.h
 * @author Microsoft Corporation
 * @brief Sas token class
 * 
 * @copyright Copyright (c) Microsoft Corporation. All rights reserved.
 * @SPDX-License-Identifier: MIT
 * 
 */

#ifndef AZIOTSASTOKEN_H
#define AZIOTSASTOKEN_H

#include <Arduino.h>
#include "Settings/config.h"
#include <az_iot_hub_client.h>
#include <az_span.h>

/**
 * @brief SAS security token class by Microsoft
 *
 * Class taken over from example code
 */
class AzIoTSasToken
{
public:
    /**
     * @brief Construct a new Az Io T Sas Token object
     *
     * @param client Client instance
     * @param deviceKey Device key string
     * @param signatureBuffer Signature string
     * @param sasTokenBuffer Token string
     */
    AzIoTSasToken(
        az_iot_hub_client *client,
        az_span deviceKey,
        az_span signatureBuffer,
        az_span sasTokenBuffer);

    /**
     * @brief Generate SAS token
     *
     * @param expiryTimeInMinutes Expire time
     * @return int Result code, 0 == Success
     */
    int Generate(unsigned int expiryTimeInMinutes);

    /**
     * @brief Get token valid
     *
     * @return true Token not valid anymore
     * @return false Token is still valid
     */
    bool IsExpired();

    /**
     * @brief Get the sas token
     *
     * @return az_span Token string
     */
    az_span Get();

private:
    az_iot_hub_client *client;   /**< Client instance */
    az_span deviceKey;           /**< Device key string */
    az_span signatureBuffer;     /**< Signature string */
    az_span sasTokenBuffer;      /**< Token string buffer */
    az_span sasToken;            /**< Token string */
    uint32_t expirationUnixTime; /**< Expiration time stamp */
};

#endif // AZIOTSASTOKEN_H
