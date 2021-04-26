library(deSolve)
library(ggplot2)
library(tidyr)

discrete_p <- "/home/thinh/Dropbox/oucru/cpp/cpp_training/output/age_location"
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
    forceInfection_0_14_HCM <- beta_0_14_HCM * (x * a * I_0_14_HCM / N_0_14_HCM + x * b * I_15_64_HCM / N_15_64_HCM +
                                             x * c * I_65_HCM / N_65_HCM + y * I_0_14_HN / N_0_14_HN +
                                             y * I_15_64_HN / N_15_64_HN + y * I_65_HN / N_65_HN)
    dS_0_14_HCM <- -forceInfection_0_14_HCM * S_0_14_HCM
    dI_0_14_HCM <- forceInfection_0_14_HCM * S_0_14_HCM - gamma_0_14_HCM * I_0_14_HCM
    dR_0_14_HCM <- gamma_0_14_HCM * I_0_14_HCM

    forceInfection_15_64_HCM <- beta_15_64_HCM * (x * b * I_0_14_HCM / N_0_14_HCM + x * d * I_15_64_HCM / N_15_64_HCM +
                                                  x * e * I_65_HCM / N_65_HCM + y * I_0_14_HN / N_0_14_HN +
                                                  y * I_15_64_HN / N_15_64_HN + y * I_65_HN / N_65_HN)
    dS_15_64_HCM <- -forceInfection_15_64_HCM * S_15_64_HCM
    dI_15_64_HCM <- forceInfection_15_64_HCM * S_15_64_HCM - gamma_15_64_HCM * I_15_64_HCM
    dR_15_64_HCM <- gamma_15_64_HCM * I_15_64_HCM

    forceInfection_65_HCM <- beta_65_HCM * (x * c * I_0_14_HCM / N_0_14_HCM + x * e * I_15_64_HCM / N_15_64_HCM +
                                                  x * f * I_65_HCM / N_65_HCM + y * I_0_14_HN / N_0_14_HN +
                                                  y * I_15_64_HN / N_15_64_HN + y * I_65_HN / N_65_HN)
    dS_65_HCM <- -forceInfection_65_HCM * S_65_HCM
    dI_65_HCM <- forceInfection_65_HCM * S_65_HCM - gamma_65_HCM * I_65_HCM
    dR_65_HCM <- gamma_65_HCM * I_65_HCM

    # HN
    forceInfection_0_14_HN <- beta_0_14_HN * (y * I_0_14_HCM / N_0_14_HCM + y * I_15_64_HCM / N_15_64_HCM +
                                                y * I_65_HCM / N_65_HCM + z * a * I_0_14_HN / N_0_14_HN +
                                                z * b * I_15_64_HN / N_15_64_HN + z * c * I_65_HN / N_65_HN)
    dS_0_14_HN <- -forceInfection_0_14_HN * S_0_14_HN
    dI_0_14_HN <- forceInfection_0_14_HN * S_0_14_HN - gamma_0_14_HN * I_0_14_HN
    dR_0_14_HN <- gamma_0_14_HN * I_0_14_HN

    forceInfection_15_64_HN <- beta_15_64_HN * (y * I_0_14_HCM / N_0_14_HCM + y * I_15_64_HCM / N_15_64_HCM +
                                                  y * I_65_HCM / N_65_HCM + z * b * I_0_14_HN / N_0_14_HN +
                                                  z * d * I_15_64_HN / N_15_64_HN + z * e * I_65_HN / N_65_HN)
    dS_15_64_HN <- -forceInfection_15_64_HN * S_15_64_HN
    dI_15_64_HN <- forceInfection_15_64_HN * S_15_64_HN - gamma_15_64_HN * I_15_64_HN
    dR_15_64_HN <- gamma_15_64_HN * I_15_64_HN

    forceInfection_65_HN <- beta_65_HN * (y * I_0_14_HCM / N_0_14_HCM + y * I_15_64_HCM / N_15_64_HCM +
                                            y * I_65_HCM / N_65_HCM + z * c * I_0_14_HN / N_0_14_HN +
                                            z * e * I_15_64_HN / N_15_64_HN + z * f * I_65_HN / N_65_HN)
    dS_65_HN <- -forceInfection_65_HN * S_65_HN
    dI_65_HN <- forceInfection_65_HN * S_65_HN - gamma_65_HN * I_65_HN
    dR_65_HN <- gamma_65_HN * I_65_HN

    return(list(c(dS_0_14_HCM, dI_0_14_HCM, dR_0_14_HCM, dS_15_64_HCM, dI_15_64_HCM, dR_15_64_HCM,
                  dS_65_HCM, dI_65_HCM, dR_65_HCM, dS_0_14_HN, dI_0_14_HN, dR_0_14_HN, dS_15_64_HN, dI_15_64_HN, dR_15_64_HN,
                  dS_65_HN, dI_65_HN, dR_65_HN)))
  })
}

# Input parameters
parameters_values <- c(
  beta_0_14_HCM <- 1.5,
  beta_15_64_HCM <- 1.5,
  beta_65_HCM <- 1.5,
  beta_0_14_HN <- 1.5,
  beta_15_64_HN <- 1.5,
  beta_65_HN <- 1.5,
  
  gamma_0_14_HCM <- 0.5,
  gamma_15_64_HCM <- 0.5,
  gamma_65_HCM <- 0.5,
  gamma_0_14_HN <- 0.5,
  gamma_15_64_HN <- 0.5,
  gamma_65_HN <- 0.5,
  
  N_0_14_HCM <- 1000,
  N_15_64_HCM <- 2000,
  N_65_HCM <- 999,
  N_0_14_HN <- 899,
  N_15_64_HN <- 699,
  N_65_HN <- 799,
  
  a <- 0.5,
  b <- 0.6,
  c <- 0.7,
  d <- 0.4,
  e <- 0.3,
  f <- 0.8,
  x <- 0.85,
  y <- 0.1,
  z <- 0.95
)

# Input initial values
initial_values <- c(
  # HCM
  S_0_14_HCM = 999,
  I_0_14_HCM =   1,
  R_0_14_HCM =   0,
  S_15_64_HCM = 1999,
  I_15_64_HCM =   1,
  R_15_64_HCM =   0,
  S_65_HCM = 999,
  I_65_HCM =   0,
  R_65_HCM =   0,
  
  # HN
  S_0_14_HN = 899,
  I_0_14_HN =   0,
  R_0_14_HN =   0,
  S_15_64_HN = 699,
  I_15_64_HN =   0,
  R_15_64_HN =   0,
  S_65_HN = 799,
  I_65_HN =   0,
  R_65_HN =   0
)

# Input time values
time_values <- seq(0, days) # days


start_time <- Sys.time()
# Solve the differential equations
sir_values_1 <- ode(
  y = initial_values,
  times = time_values,
  func = sir_equations,
  parms = parameters_values
)

end_time <- Sys.time()

runtime <- end_time - start_time
runtime

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
df_plot$Compartment <- factor(df_plot$Compartment, 
                              levels = c("S_0_14", "S_15_64", "S_65", 
                                         "I_0_14", "I_15_64", "I_65",
                                         "R_0_14", "R_15_64", "R_65"))
ggplot(df_plot, aes(x = time, y = Value, col = Compartment)) +
  geom_line(size = 1) + facet_wrap(~ location + package) +
  theme_minimal() +
  theme(text = element_text(size = 14))

# ggsave(file.path(outp, "ageLocation_discreteODE.pdf"), width = 7, height = 4)

# View data frame to compare
df2[,grep("_0_14_HN", colnames(df2))]
