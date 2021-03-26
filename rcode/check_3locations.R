S_HCM <- 999
I_HCM <- 1
R_HCM <- 0
beta_HCM <- 1.5
N_HCM <- S_HCM[1] + I_HCM[1] + R_HCM[1]
gamma_HCM <- 0.5

S_HN <- 699
I_HN <- 0
R_HN <- 0
beta_HN <- 1.2
N_HN <- S_HN[1] + I_HN[1] + R_HN[1]
gamma_HN <- 0.3

S_DN <- 899
I_DN <- 0
R_DN <- 0
beta_DN <- 1.8
N_DN <- S_DN[1] + I_DN[1] + R_DN[1]
gamma_DN <- 0.4

# x = HCM-HN, y = HCM-HCM, z = HN-HN
hcm_hn <- 0.1
hcm_dn <- 0.2
hn_dn <- 0.3
self <- 1

for (i in 1:50) {
  forceInfection_HCM <- beta_HCM * (self * I_HCM[i] / N_HCM + hcm_hn * I_HN[i] / N_HN + hcm_dn * I_DN[i] / N_DN)
  dS_HCM <- -forceInfection_HCM * S_HCM[i]
  S_HCM[i + 1] <- S_HCM[i] + dS_HCM
  dI_HCM <- forceInfection_HCM * S_HCM[i] - gamma_HCM * I_HCM[i]
  I_HCM[i + 1] <- I_HCM[i] + dI_HCM
  dR_HCM <- gamma_HCM * I_HCM[i]
  R_HCM[i + 1] <- R_HCM[i] + dR_HCM
  
  forceInfection_HN <- beta_HN * (self * I_HN[i] / N_HN + hcm_hn * I_HCM[i] / N_HCM + hn_dn * I_DN[i] / N_DN)
  dS_HN <- -forceInfection_HN * S_HN[i]
  S_HN[i + 1] <- S_HN[i] + dS_HN
  dI_HN <- forceInfection_HN * S_HN[i] - gamma_HN * I_HN[i]
  I_HN[i + 1] <- I_HN[i] + dI_HN
  dR_HN <- gamma_HN * I_HN[i]
  R_HN[i + 1] <- R_HN[i] + dR_HN
  
  forceInfection_DN <- beta_DN * (self * I_DN[i] / N_DN + hcm_dn * I_HCM[i] / N_HCM + hn_dn * I_HN[i] / N_HN)
  dS_DN <- -forceInfection_DN * S_DN[i]
  S_DN[i + 1] <- S_DN[i] + dS_DN
  dI_DN <- forceInfection_DN * S_DN[i] - gamma_DN * I_DN[i]
  I_DN[i + 1] <- I_DN[i] + dI_DN
  dR_DN <- gamma_DN * I_DN[i]
  R_DN[i + 1] <- R_DN[i] + dR_DN
}

df_HCM <- as.data.frame(cbind(S_HCM, I_HCM, R_HCM))
df_HCM$sum_HCM <- rowSums(df_HCM)

df_HN <- as.data.frame(cbind(S_HN, I_HN, R_HN))
df_HN$sum_HN <- rowSums(df_HN)

df_DN <- as.data.frame(cbind(S_DN, I_DN, R_DN))
df_DN$sum_DN <- rowSums(df_DN)

df <- cbind(df_HCM, df_HN, df_DN)
head(df)

