library(deSolve)
library(ggplot2)
library(tidyr)

discrete_p <- "/home/thinh/Dropbox/oucru/cpp/cpp_training/output/location"
outp <- "/home/thinh/Dropbox/oucru/cpp/plots"

days <- 20

# Time step
timeStep <- 0.001
timeStart <- 1 / timeStep + 2
timeEnd <- days / timeStep + 2

# Make the SIR model
sir_equations <- function(time, variables, parameters) {
  with(as.list(c(variables, parameters)), {
    forceInfection_HCM <- beta_HCM * (y * I_HCM / N_HCM + x * I_HN / N_HN)
    dS_HCM <- -forceInfection_HCM * S_HCM
    dI_HCM <- forceInfection_HCM * S_HCM - gamma_HCM * I_HCM
    dR_HCM <- gamma_HCM * I_HCM
    
    forceInfection_HN <- beta_HN * (z * I_HN / N_HN + x * I_HCM / N_HCM)
    dS_HN <- -forceInfection_HN * S_HN
    dI_HN <- forceInfection_HN * S_HN - gamma_HN * I_HN
    dR_HN <- gamma_HN * I_HN
    return(list(c(dS_HCM, dI_HCM, dR_HCM, dS_HN, dI_HN, dR_HN)))
  })
}

# Input parameters
parameters_values <- c(
  beta_HCM <- 1.5,
  gamma_HCM <- 0.3,
  N_HCM <- 1000,
  beta_HN <- 1.2,
  gamma_HN <- 0.5,
  N_HN <- 899,
  x <- 0.1,
  y <- 0.85,
  z <- 0.95
)

# Input initial values
initial_values <- c(
  S_HCM = 999,
  I_HCM =   1,
  R_HCM =   0,
  S_HN = 899,
  I_HN =   0,
  R_HN =   0
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

ggsave(file.path(outp, "location_discreteODE.pdf"), width = 7, height = 4)

# View data frame to compare
df2[,grep("HN", colnames(df2))]

