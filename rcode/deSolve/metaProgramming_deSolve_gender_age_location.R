library(deSolve)
library(ggplot2)
library(tidyr)

discrete_p <- "/home/thinh/Dropbox/oucru/cpp/cpp_training/output/gender_age_location"
outp <- "/home/thinh/Dropbox/oucru/cpp/plots"

days <- 20

# Time step
timeStep <- 0.001
timeStart <- 1 / timeStep + 2
timeEnd <- days / timeStep + 2

source("/home/thinh/Dropbox/oucru/cpp/cpp_training/rcode/snippets/sir_equations_maker.R")
ageGroup <- c("0_14", "15_64", "65")
locGroup <- c("HCM", "HN")
sexGroup <- c("male", "female")
loc_matrix <- matrix(c(0.85, 0.1,
                       0.1, 0.95), nrow = 2)
age_matrix <- matrix(c(0.5, 0.6, 0.7, 
                       0.6, 0.4, 0.3, 
                       0.7, 0.3, 0.8), nrow = 3)
sex_matrix <- matrix(c(0.5, 0.6,
                       0.6, 0.7), nrow = 2)

contact_matrix <- kronecker(age_matrix, sex_matrix)
contact_matrix <- kronecker(loc_matrix, contact_matrix)
sir_equations_maker(sir_equations, contact_matrix,sexGroup, ageGroup, locGroup)

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
