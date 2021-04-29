library(deSolve)
library(ggplot2)
library(tidyr)

discrete_p <- "/home/thinh/Downloads/cpp_training/output/gender_age_location"
outp <- "/home/thinh/Dropbox/oucru/cpp/plots"

days <- 20

# Time step
timeStep <- 0.001
timeStart <- 1 / timeStep + 2
timeEnd <- days / timeStep + 2

# Make the SIR model
sir_equations <- function(time, variables, parameters) {
  with(as.list(c(variables, parameters)), {
    # HCM
    forceInfection_male_0_14_HCM <- beta_male_0_14_HCM * (k * x * a * I_male_0_14_HCM / N_male_0_14_HCM + k * x * b * I_female_0_14_HCM / N_female_0_14_HCM +
                                                            k * y * I_male_15_64_HCM / N_male_15_64_HCM + k * y * I_female_15_64_HCM / N_female_15_64_HCM +
                                                            k * z * I_male_65_HCM / N_male_65_HCM + k * z * I_female_65_HCM / N_female_65_HCM +
                                                            m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                            m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                            m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_male_0_14_HCM <- -forceInfection_male_0_14_HCM * S_male_0_14_HCM
    dI_male_0_14_HCM <- forceInfection_male_0_14_HCM * S_male_0_14_HCM - gamma_male_0_14_HCM * I_male_0_14_HCM
    dR_male_0_14_HCM <- gamma_male_0_14_HCM * I_male_0_14_HCM

    forceInfection_female_0_14_HCM <- beta_female_0_14_HCM * (k * x * b * I_male_0_14_HCM / N_male_0_14_HCM + k * x * c * I_female_0_14_HCM / N_female_0_14_HCM +
                                                                k * y * I_male_15_64_HCM / N_male_15_64_HCM + k * y * I_female_15_64_HCM / N_female_15_64_HCM +
                                                                k * z * I_male_65_HCM / N_male_65_HCM + k * z * I_female_65_HCM / N_female_65_HCM +
                                                                m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                                m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                                m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_female_0_14_HCM <- -forceInfection_female_0_14_HCM * S_female_0_14_HCM
    dI_female_0_14_HCM <- forceInfection_female_0_14_HCM * S_female_0_14_HCM - gamma_female_0_14_HCM * I_female_0_14_HCM
    dR_female_0_14_HCM <- gamma_female_0_14_HCM * I_female_0_14_HCM

    forceInfection_male_15_64_HCM <- beta_male_15_64_HCM * (k * y * I_male_0_14_HCM / N_male_0_14_HCM + k * y * I_female_0_14_HCM / N_female_0_14_HCM +
                                                              k * t * a * I_male_15_64_HCM / N_male_15_64_HCM + k * t * b * I_female_15_64_HCM / N_female_15_64_HCM +
                                                              k * q * I_male_65_HCM / N_male_65_HCM + k * q * I_female_65_HCM / N_female_65_HCM +
                                                              m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                              m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                              m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_male_15_64_HCM <- -forceInfection_male_15_64_HCM * S_male_15_64_HCM
    dI_male_15_64_HCM <- forceInfection_male_15_64_HCM * S_male_15_64_HCM - gamma_male_15_64_HCM * I_male_15_64_HCM
    dR_male_15_64_HCM <- gamma_male_15_64_HCM * I_male_15_64_HCM
    
    forceInfection_female_15_64_HCM <- beta_female_15_64_HCM * (k * y * I_male_0_14_HCM / N_male_0_14_HCM + k * y * I_female_0_14_HCM / N_female_0_14_HCM +
                                                                  k * t * b * I_male_15_64_HCM / N_male_15_64_HCM + k * t * c * I_female_15_64_HCM / N_female_15_64_HCM +
                                                                  k * q * I_male_65_HCM / N_male_65_HCM + k * q * I_female_65_HCM / N_female_65_HCM +
                                                                  m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                                  m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                                  m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_female_15_64_HCM <- -forceInfection_female_15_64_HCM * S_female_15_64_HCM
    dI_female_15_64_HCM <- forceInfection_female_15_64_HCM * S_female_15_64_HCM - gamma_female_15_64_HCM * I_female_15_64_HCM
    dR_female_15_64_HCM <- gamma_female_15_64_HCM * I_female_15_64_HCM
    
    forceInfection_male_65_HCM <- beta_male_65_HCM * (k * z * I_male_0_14_HCM / N_male_0_14_HCM + k * z * I_female_0_14_HCM / N_female_0_14_HCM +
                                                        k * q * I_male_15_64_HCM / N_male_15_64_HCM + k * q * I_female_15_64_HCM / N_female_15_64_HCM +
                                                        k * r * a * I_male_65_HCM / N_male_65_HCM + k * r * b * I_female_65_HCM / N_female_65_HCM +
                                                        m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                        m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                        m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_male_65_HCM <- -forceInfection_male_65_HCM * S_male_65_HCM
    dI_male_65_HCM <- forceInfection_male_65_HCM * S_male_65_HCM - gamma_male_65_HCM * I_male_65_HCM
    dR_male_65_HCM <- gamma_male_65_HCM * I_male_65_HCM
    
    forceInfection_female_65_HCM <- beta_female_65_HCM * (k * z * I_male_0_14_HCM / N_male_0_14_HCM + k * z * I_female_0_14_HCM / N_female_0_14_HCM +
                                                            k * q * I_male_15_64_HCM / N_male_15_64_HCM + k * q * I_female_15_64_HCM / N_female_15_64_HCM +
                                                            k * r * b * I_male_65_HCM / N_male_65_HCM + k * r * c * I_female_65_HCM / N_female_65_HCM +
                                                            m * I_male_0_14_HN / N_male_0_14_HN + m * I_female_0_14_HN / N_female_0_14_HN +
                                                            m * I_male_15_64_HN / N_male_15_64_HN + m * I_female_15_64_HN / N_female_15_64_HN +
                                                            m * I_male_65_HN / N_male_65_HN + m * I_female_65_HN / N_female_65_HN)
    dS_female_65_HCM <- -forceInfection_female_65_HCM * S_female_65_HCM
    dI_female_65_HCM <- forceInfection_female_65_HCM * S_female_65_HCM - gamma_female_65_HCM * I_female_65_HCM
    dR_female_65_HCM <- gamma_female_65_HCM * I_female_65_HCM

    # HN
    forceInfection_male_0_14_HN <- beta_male_0_14_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                          m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                          m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                          n * x * a * I_male_0_14_HN / N_male_0_14_HN + n * x * b * I_female_0_14_HN / N_female_0_14_HN +
                                                          n * y * I_male_15_64_HN / N_male_15_64_HN + n * y * I_female_15_64_HN / N_female_15_64_HN +
                                                          n * z * I_male_65_HN / N_male_65_HN + n * z * I_female_65_HN / N_female_65_HN)
    dS_male_0_14_HN <- -forceInfection_male_0_14_HN * S_male_0_14_HN
    dI_male_0_14_HN <- forceInfection_male_0_14_HN * S_male_0_14_HN - gamma_male_0_14_HN * I_male_0_14_HN
    dR_male_0_14_HN <- gamma_male_0_14_HN * I_male_0_14_HN

    forceInfection_female_0_14_HN <- beta_female_0_14_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                              m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                              m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                              n * x * b * I_male_0_14_HN / N_male_0_14_HN + n * x * c * I_female_0_14_HN / N_female_0_14_HN +
                                                              n * y * I_male_15_64_HN / N_male_15_64_HN + n * y * I_female_15_64_HN / N_female_15_64_HN +
                                                              n * z * I_male_65_HN / N_male_65_HN + n * z * I_female_65_HN / N_female_65_HN)
    dS_female_0_14_HN <- -forceInfection_female_0_14_HN * S_female_0_14_HN
    dI_female_0_14_HN <- forceInfection_female_0_14_HN * S_female_0_14_HN - gamma_female_0_14_HN * I_female_0_14_HN
    dR_female_0_14_HN <- gamma_female_0_14_HN * I_female_0_14_HN

    forceInfection_male_15_64_HN <- beta_male_15_64_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                            m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                            m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                            n * y * I_male_0_14_HN / N_male_0_14_HN + n * y * I_female_0_14_HN / N_female_0_14_HN +
                                                            n * t * a * I_male_15_64_HN / N_male_15_64_HN + n * t * b * I_female_15_64_HN / N_female_15_64_HN +
                                                            n * q * I_male_65_HN / N_male_65_HN + n * q * I_female_65_HN / N_female_65_HN)
    dS_male_15_64_HN <- -forceInfection_male_15_64_HN * S_male_15_64_HN
    dI_male_15_64_HN <- forceInfection_male_15_64_HN * S_male_15_64_HN - gamma_male_15_64_HN * I_male_15_64_HN
    dR_male_15_64_HN <- gamma_male_15_64_HN * I_male_15_64_HN
    
    forceInfection_female_15_64_HN <- beta_female_15_64_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                                m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                                m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                                n * y * I_male_0_14_HN / N_male_0_14_HN + n * y * I_female_0_14_HN / N_female_0_14_HN +
                                                                n * t * b * I_male_15_64_HN / N_male_15_64_HN + n * t * c * I_female_15_64_HN / N_female_15_64_HN +
                                                                n * q * I_male_65_HN / N_male_65_HN + n * q * I_female_65_HN / N_female_65_HN)
    dS_female_15_64_HN <- -forceInfection_female_15_64_HN * S_female_15_64_HN
    dI_female_15_64_HN <- forceInfection_female_15_64_HN * S_female_15_64_HN - gamma_female_15_64_HN * I_female_15_64_HN
    dR_female_15_64_HN <- gamma_female_15_64_HN * I_female_15_64_HN
    
    forceInfection_male_65_HN <- beta_male_65_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                      m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                      m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                      n * z * I_male_0_14_HN / N_male_0_14_HN + n * z * I_female_0_14_HN / N_female_0_14_HN +
                                                      n * q * I_male_15_64_HN / N_male_15_64_HN + n * q * I_female_15_64_HN / N_female_15_64_HN +
                                                      n * r * a * I_male_65_HN / N_male_65_HN + n * r * b * I_female_65_HN / N_female_65_HN)
    dS_male_65_HN <- -forceInfection_male_65_HN * S_male_65_HN
    dI_male_65_HN <- forceInfection_male_65_HN * S_male_65_HN - gamma_male_65_HN * I_male_65_HN
    dR_male_65_HN <- gamma_male_65_HN * I_male_65_HN
    
    forceInfection_female_65_HN <- beta_female_65_HN * (m * I_male_0_14_HCM / N_male_0_14_HCM + m * I_female_0_14_HCM / N_female_0_14_HCM +
                                                          m * I_male_15_64_HCM / N_male_15_64_HCM + m * I_female_15_64_HCM / N_female_15_64_HCM +
                                                          m * I_male_65_HCM / N_male_65_HCM + m * I_female_65_HCM / N_female_65_HCM +
                                                          n * z * I_male_0_14_HN / N_male_0_14_HN + n * z * I_female_0_14_HN / N_female_0_14_HN +
                                                          n * q * I_male_15_64_HN / N_male_15_64_HN + n * q * I_female_15_64_HN / N_female_15_64_HN +
                                                          n * r * b * I_male_65_HN / N_male_65_HN + n * r * c * I_female_65_HN / N_female_65_HN)
    dS_female_65_HN <- -forceInfection_female_65_HN * S_female_65_HN
    dI_female_65_HN <- forceInfection_female_65_HN * S_female_65_HN - gamma_female_65_HN * I_female_65_HN
    dR_female_65_HN <- gamma_female_65_HN * I_female_65_HN

    return(list(c(dS_male_0_14_HCM, dI_male_0_14_HCM, dR_male_0_14_HCM, dS_female_0_14_HCM, dI_female_0_14_HCM, dR_female_0_14_HCM,
                  dS_male_15_64_HCM, dI_male_15_64_HCM, dR_male_15_64_HCM, dS_female_15_64_HCM, dI_female_15_64_HCM, dR_female_15_64_HCM,
                  dS_male_65_HCM, dI_male_65_HCM, dR_male_65_HCM, dS_female_65_HCM, dI_female_65_HCM, dR_female_65_HCM,
                  dS_male_0_14_HN, dI_male_0_14_HN, dR_male_0_14_HN, dS_female_0_14_HN, dI_female_0_14_HN, dR_female_0_14_HN, 
                  dS_male_15_64_HN, dI_male_15_64_HN, dR_male_15_64_HN, dS_female_15_64_HN, dI_female_15_64_HN, dR_female_15_64_HN,
                  dS_male_65_HN, dI_male_65_HN, dR_male_65_HN, dS_female_65_HN, dI_female_65_HN, dR_female_65_HN)))
  })
}

# Input parameters
parameters_values <- c(
  beta_male_0_14_HCM <- 1.5,
  beta_female_0_14_HCM <- 1.5,
  beta_male_15_64_HCM <- 1.5,
  beta_female_15_64_HCM <- 1.5,
  beta_male_65_HCM <- 1.5,
  beta_female_65_HCM <- 1.5,
  beta_male_0_14_HN <- 1.5,
  beta_female_0_14_HN <- 1.5,
  beta_male_15_64_HN <- 1.5,
  beta_female_15_64_HN <- 1.5,
  beta_male_65_HN <- 1.5,
  beta_female_65_HN <- 1.5,
  
  gamma_male_0_14_HCM <- 0.3,
  gamma_female_0_14_HCM <- 0.5,
  gamma_male_15_64_HCM <- 0.2,
  gamma_female_15_64_HCM <- 0.3,
  gamma_male_65_HCM <- 0.25,
  gamma_female_65_HCM <- 0.15,
  gamma_male_0_14_HN <- 0.5,
  gamma_female_0_14_HN <- 0.35,
  gamma_male_15_64_HN <- 0.45,
  gamma_female_15_64_HN <- 0.5,
  gamma_male_65_HN <- 0.25,
  gamma_female_65_HN <- 0.15,
  
  N_male_0_14_HCM <- 1000,
  N_female_0_14_HCM <- 1299,
  N_male_15_64_HCM <- 1599,
  N_female_15_64_HCM <- 2000,
  N_male_65_HCM <- 1399,
  N_female_65_HCM <- 1499,
  N_male_0_14_HN <- 899,
  N_female_0_14_HN <- 299,
  N_male_15_64_HN <- 699,
  N_female_15_64_HN <- 599,
  N_male_65_HN <- 799,
  N_female_65_HN <- 399,
  
  a <- 0.5,
  b <- 0.6,
  c <- 0.7,
  x <- 0.5,
  y <- 0.6,
  z <- 0.7,
  t <- 0.4,
  q <- 0.3,
  r <- 0.8,
  k <- 0.85,
  m <- 0.1,
  n <- 0.95
)

# Input initial values
initial_values <- c(
  # HCM
  S_male_0_14_HCM = 999,
  I_male_0_14_HCM =   1,
  R_male_0_14_HCM =   0,
  S_female_0_14_HCM = 1299,
  I_female_0_14_HCM =   0,
  R_female_0_14_HCM =   0,
  S_male_15_64_HCM = 1599,
  I_male_15_64_HCM =   0,
  R_male_15_64_HCM =   0,
  S_female_15_64_HCM = 1999,
  I_female_15_64_HCM = 1,
  R_female_15_64_HCM = 0,
  S_male_65_HCM = 1399,
  I_male_65_HCM =   0,
  R_male_65_HCM =   0,
  S_female_65_HCM = 1499,
  I_female_65_HCM =   0,
  R_female_65_HCM =   0,
  
  # HN
  S_male_0_14_HN = 899,
  I_male_0_14_HN =   0,
  R_male_0_14_HN =   0,
  S_female_0_14_HN = 299,
  I_female_0_14_HN =   0,
  R_female_0_14_HN =   0,
  S_male_15_64_HN = 699,
  I_male_15_64_HN =   0,
  R_male_15_64_HN =   0,
  S_female_15_64_HN = 599,
  I_female_15_64_HN =   0,
  R_female_15_64_HN =   0,
  S_male_65_HN = 799,
  I_male_65_HN =   0,
  R_male_65_HN =   0,
  S_female_65_HN = 399,
  I_female_65_HN =   0,
  R_female_65_HN =   0
)

# Input time values
time_values <- seq(0, days) # days

# Solve the differential equations
sir_values_1 <- ode(
  y = initial_values,
  times = time_values,
  func = sir_equations,
  parms = parameters_values
)

# Make data frame from deSolve results
sir_values_1 <- data.frame(sir_values_1)
colnames(sir_values_1)[-1] <- paste0(colnames(sir_values_1)[-1], "_deSolve")

# Load our discrete time results
discrete_path <- list.files(discrete_p, pattern = ".csv", full.names = T)
l <- lapply(discrete_path, function(x) read.csv(x, header = T, stringsAsFactors = F, check.names = F))
df <- Reduce(function(x, y) merge(x, y, by = "Time", all = TRUE), l)

# Only get the time step we want
times <- seq(timeStart, timeEnd, 1 / timeStep)
df <- df[c(1, times),]
colnames(df)[-1] <- paste0(colnames(df)[-1], "_discrete")
df$Time[-1] <- (df$Time[-1] - 1) * timeStep

# Merge discrete data frame with deSolve data frame and visualize
df2 <- merge(sir_values_1, df, by.x = "time", by.y = "Time")
df_plot <- gather(df2, key = "Compartment", value = "Value", grep("time", colnames(df2), invert = T))
df_plot$package <- sapply(df_plot$Compartment, function(x) {a <- strsplit(x, "_")[[1]]; b <- a[length(a)]; return(b)})
df_plot$Compartment <- gsub("_deSolve|_discrete", "", df_plot$Compartment)
df_plot$location <- sapply(df_plot$Compartment, function(x) {a <- strsplit(x, "_")[[1]]; b <- a[length(a)]; return(b)})
df_plot$Compartment <- gsub("_HCM|_HN", "", df_plot$Compartment)
df_plot$Compartment <- factor(df_plot$Compartment)
ggplot(df_plot, aes(x = time, y = Value, col = Compartment)) +
  geom_line(size = 1) + facet_wrap(~ location + package) +
  theme_minimal() +
  theme(text = element_text(size = 14))

ggsave(file.path(outp, "genderAgeLocation_discreteODE.pdf"), width = 7, height = 5)

# View data frame to compare
df2[,grep("_female_65_HCM", colnames(df2))]
